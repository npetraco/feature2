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
  
  //??
  /* 1 is the fastest polling, it means 1 kHz polling */
  if( ftdi_set_latency_timer(&ftdic, 1) < 0 ) {
    //fprintf(stderr, "Failed to set latency timer (%s).\n", ftdi_get_error_string(&ftdic));
    Rcpp::Rcerr << "Failed to set latency timer: "<< ftdi_get_error_string(&ftdic) << endl;
    error();
  }
  
  ftdic_latency_set = true;
  
  if( ftdi_set_bitmode(&ftdic, 0xff, BITMODE_MPSSE) < 0 ) {
    //fprintf(stderr, "Failed set BITMODE_MPSSE on iCE FTDI USB device.\n");
    Rcpp::Rcerr << "Failed set BITMODE_MPSSE on iCE FTDI USB device." << endl;
    error();
  }
  
  Rcpp::Rcerr << "FTDI bitmode set." << endl;
  
  // Exit
  Rcpp::Rcerr << "Exiting..." << endl;
  ftdi_disable_bitbang(&ftdic);
  ftdi_usb_close(&ftdic);
  ftdi_deinit(&ftdic);
  Rcpp::Rcerr << "Bye!" << endl;
  
}