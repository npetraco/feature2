#include <Rcpp.h>

#include <ftdi.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace Rcpp;
using namespace std;

struct ftdi_context ftdic;
bool ftdic_open = false;
bool verbose = false;
bool ftdic_latency_set = false;
unsigned char ftdi_latency;

// Below is from iceprog.c in Clifford Wolf's icestorm suite:
// https://github.com/cliffordwolf/icestorm

// [[Rcpp::export]]
void inputByteCheck(unsigned int abyte) {
  
  if(abyte > 255) {
    Rcpp::Rcerr << "Caution: Input > 255!" << endl;    
  }
  
  std::bitset<sizeof(uint8_t) * CHAR_BIT > bs((uint8_t) abyte);
  Rcpp::Rcout << "A byte?:         " << bs.to_string() << endl;
  //Rcpp::Rcout << "Size:            " << bs.size() << endl;
  Rcpp::Rcout << "As hexidecimal:  " << hex << abyte << endl;
  Rcpp::Rcout << "As unsigned int: " << dec << abyte << endl;
  
}

// [[Rcpp::export]]
void inputStringByteCheck(std::string abyte_string) {
  
  long ii = std::stol(abyte_string, nullptr, 2);
  uint8_t iiu = std::stol(abyte_string, nullptr, 2); // This is what we send as data to libftdi
  std::bitset<sizeof(uint8_t) * CHAR_BIT > bs((uint8_t) iiu);
  
  Rcpp::Rcout << "Input string:             " << abyte_string << endl;
  Rcpp::Rcout << "A iiu byte?:              " << bs.to_string() << endl;
  Rcpp::Rcout << "As hexidecimal:           " << hex << ii << endl;
  Rcpp::Rcout << "As iiu hexidecimal?:      " << hex << bs.to_ulong() << endl;
  Rcpp::Rcout << "As unsigned dec:          " << dec << ii << endl;
  
}

// ????
void check_rx()
{
  while (1) {
    uint8_t data;
    int rc = ftdi_read_data(&ftdic, &data, 1);
    if (rc <= 0) break;
    //fprintf(stderr, "unexpected rx byte: %02X\n", data);
    std::bitset<sizeof(uint8_t) * CHAR_BIT > data_bs((uint8_t) data);
    Rcpp::Rcerr << "unexpected rx byte: " << data_bs.to_string() << endl;
  }
}

// Throw error and close connection function
void error()
{
  check_rx();
  Rcpp::Rcerr << "ABORT." << endl;
  //fprintf(stderr, "ABORT.\n");
  if (ftdic_open)
    ftdi_usb_close(&ftdic);
  ftdi_deinit(&ftdic);
  exit(1);
}

//
uint8_t recv_byte()
{
  uint8_t data;
  while (1) {
    int rc = ftdi_read_data(&ftdic, &data, 1);
    if (rc < 0) {
      //fprintf(stderr, "Read error.\n");
      Rcpp::Rcerr << "Read error." << endl;
      error();
    }
    if (rc == 1)
      break;
    usleep(100);
  }
  return data;
}

//
void send_byte(uint8_t data)
{
  int rc = ftdi_write_data(&ftdic, &data, 1);
  if (rc != 1) {
    //fprintf(stderr, "Write error (single byte, rc=%d, expected %d).\n", rc, 1);
    Rcpp::Rcerr << "Write error (single byte, rc = " << rc << ", expected: " << 1 << endl; 
    error();
  }
}

// ??
int get_cdone()
{
  uint8_t data;
  send_byte(0x81);
  data = recv_byte();
  // ADBUS6 (GPIOL2)
  return (data & 0x40) != 0;
}

// ??
void set_gpio(int a, int b, int c)
{
  // sets two bytes - intial led_values and i/o direction
  
  // what does the one indicate?
  // one is the clock ... maybe set high by default?
  uint8_t gpio = 1;
  
  if (a) {
    // (GPIOL0)
    gpio |= 0x10;    // 1 << 4
  }
  if (b) {
    // (GPIOL1)
    gpio |= 0x20;    // 1 << 5
  }
  if (c) {
    // (GPIOL2)
    gpio |= 0x40;    // 1 << 6
  }
  // GPIOL3 not connected to fpga.
  
  /*
  At this point, the MPSSE is ready for commands .  Each command consists of an
  op - code followed by any necessary parameters or data.
  The following commands are used to set the initial direction and logic state of
  the pins when first entering MPSSE mode. They are also use to set or read
  GPIO pins.
  0x80 is a op-code to set initial led_values and direction.
  */
  
  send_byte(0x80);       // must be a command,,,
  send_byte(gpio);
  
  // 0xfb.toString(2);
  // 0xFB = "11111011"  should be correct enables miso as only input
  send_byte(0xFB);        // this is the direction a bit mask?
}

// ??
void send_spi(uint8_t *data, int n)
{
  if (n < 1)
    return;
  
  // max size check...
  // 3.3.2 Clock Data Bytes Out on - ve clo ck edge MSB first  (no read) Use if CLK starts at '0
  
  send_byte(0x11);
  send_byte(n-1);
  send_byte((n-1) >> 8);
  
  int rc = ftdi_write_data(&ftdic, data, n);
  if (rc != n) {
    //fprintf(stderr, "Write error (chunk, rc=%d, expected %d).\n", rc, n);
    Rcpp::Rcerr << "Write error (chunk, rc = " << rc << ", expected: " << n << endl;
    error();
  }
}

// Send in data as a string literal of 8 bits:
// [[Rcpp::export]]
void send_byte_string(std::string data_byte_as_string)
{
  uint8_t data = std::stol(data_byte_as_string, nullptr, 2);
  int rc = ftdi_write_data(&ftdic, &data, 1);
  if (rc != 1) {
    Rcpp::Rcerr << "Write error (single byte, rc= " << rc << ", expected " << 1 << ")."<<endl;
    //fprintf(stderr, "Write error (single byte, rc=%d, expected %d).\n", rc, 1);
    error();
  }
}

// [[Rcpp::export]]
void ftdi_event() {
  
  enum ftdi_interface ifnum = INTERFACE_B;
  
  ftdi_init(&ftdic);
  Rcpp::Rcerr << "Init... Hi!" << endl;
  
  ftdi_set_interface(&ftdic, ifnum);
  //Rcpp::Rcerr << "Got here..." << endl;
  
  if( ftdi_usb_open(&ftdic, 0x0403, 0x6010) ) {
    //fprintf(stderr, "Can't find iCE FTDI USB device (vedor_id 0x0403, device_id 0x6010).\n");
    Rcpp::Rcerr << "Can't find iCE FTDI USB device (vedor_id 0x0403, device_id 0x6010)." << endl;
    error();
  }
  
  ftdic_open = true;
  Rcpp::Rcerr << "FTDI device open." << endl;
  
  if( ftdi_usb_reset(&ftdic) ) {
    //fprintf(stderr, "Failed to reset iCE FTDI USB device.\n");
    Rcpp::Rcerr << "Failed to reset iCE FTDI USB device." << endl;
    error();
  }
  
  Rcpp::Rcerr << "FTDI device reset." << endl;
  
  if( ftdi_usb_purge_buffers(&ftdic) ) {
    //fprintf(stderr, "Failed to purge buffers on iCE FTDI USB device.\n");
    Rcpp::Rcerr << "Failed to purge buffers on iCE FTDI USB device." << endl;
    error();
  }
  
  Rcpp::Rcerr << "FTDI device buffers purged." << endl;
  
  //??
  if( ftdi_get_latency_timer(&ftdic, &ftdi_latency ) < 0) {
    //fprintf(stderr, "Failed to get latency timer (%s).\n", ftdi_get_error_string(&ftdic));
    Rcpp::Rcerr << "Failed to get latency timer: "<< ftdi_get_error_string(&ftdic) << endl;
    error();
  }
  
  Rcpp::Rcerr << "Latency timer get-ed." << endl;
  
  //??
  /* 1 is the fastest polling, it means 1 kHz polling */
  if( ftdi_set_latency_timer(&ftdic, 1) < 0 ) {
    //fprintf(stderr, "Failed to set latency timer (%s).\n", ftdi_get_error_string(&ftdic));
    Rcpp::Rcerr << "Failed to set latency timer: "<< ftdi_get_error_string(&ftdic) << endl;
    error();
  }
  
  ftdic_latency_set = true;
  Rcpp::Rcerr << "Latency timer set." << endl;
  
  if( ftdi_set_bitmode(&ftdic, 0xff, BITMODE_MPSSE) < 0 ) {
    //fprintf(stderr, "Failed set BITMODE_MPSSE on iCE FTDI USB device.\n");
    Rcpp::Rcerr << "Failed set BITMODE_MPSSE on iCE FTDI USB device." << endl;
    error();
  }
  
  Rcpp::Rcerr << "FTDI bitmode set." << endl;
  
  // ??
  // enable clock divide by 5
  send_byte(0x8b);
  
  // ??
  // set 6 MHz clock
  send_byte(0x86);
  send_byte(0x02);   // 1 or 2 MHz ?
  send_byte(0x00);
  
  //fprintf(stderr, "cdone: %s\n", get_cdone() ? "high" : "low");
  const char * hlm = get_cdone() ? "high" : "low";
  Rcpp::Rcerr << "cdone #1: " << hlm << endl;
  
  // initialize data out
  set_gpio(1, 1, 1);

  hlm = get_cdone() ? "high" : "low";
  Rcpp::Rcerr << "cdone #2: " << hlm << endl;
  
  int led_value = 1; // Light on??
  uint8_t data[1];

  //fprintf(stderr, "writing data led_value %d\n", led_value);
  Rcpp::Rcerr << "writing data led_value: " << led_value << endl;
  //led_value = !led_value;
  
  set_gpio(0, 1, 1);
  
  if(led_value == 1)
    data[0] = 0xcc;
  else if(led_value == 0)
    data[0] = 0xdd;
  
  // write
  send_spi( data, 1 );
  
  // deassert CS
  set_gpio(1, 1, 1);
  
  //fprintf(stderr, "done\n");
  Rcpp::Rcerr << "Done led data write." << endl;

  // Exit
  Rcpp::Rcerr << "Exiting..." << endl;
  ftdi_disable_bitbang(&ftdic);
  ftdi_usb_close(&ftdic);
  ftdi_deinit(&ftdic);
  free(data);
  Rcpp::Rcerr << "Bye!" << endl;
  
}