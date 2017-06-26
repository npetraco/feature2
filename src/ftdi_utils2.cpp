#include <Rcpp.h>

#define _GNU_SOURCE

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

void check_rx()
{
  while (1) {
    uint8_t data;
    int rc = ftdi_read_data(&ftdic, &data, 1);
    if (rc <= 0) break;
    fprintf(stderr, "unexpected rx byte: %02X\n", data);
  }
}

void error()
{
  check_rx();
  fprintf(stderr, "ABORT.\n");
  if (ftdic_open)
    ftdi_usb_close(&ftdic);
  ftdi_deinit(&ftdic);
  exit(1);
}

uint8_t recv_byte()
{
  uint8_t data;
  while (1) {
    int rc = ftdi_read_data(&ftdic, &data, 1);
    if (rc < 0) {
      fprintf(stderr, "Read error.\n");
      error();
    }
    if (rc == 1)
      break;
    usleep(100);
  }
  return data;
}

void send_byte(uint8_t data)
{
  int rc = ftdi_write_data(&ftdic, &data, 1);
  if (rc != 1) {
    fprintf(stderr, "Write error (single byte, rc=%d, expected %d).\n", rc, 1);
    error();
  }
}

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
    fprintf(stderr, "Write error (chunk, rc=%d, expected %d).\n", rc, n);
    error();
  }
}

void xfer_spi(uint8_t *data, int n)
{
  if (n < 1)
    return;
  
  send_byte(0x31);
  send_byte(n-1);
  send_byte((n-1) >> 8);
  
  int rc = ftdi_write_data(&ftdic, data, n);
  if (rc != n) {
    fprintf(stderr, "Write error (chunk, rc=%d, expected %d).\n", rc, n);
    error();
  }
  
  for (int i = 0; i < n; i++)
    data[i] = recv_byte();
}




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
  the pins when first entering MPSSE mode. They are also use to set or re a d
  GPIO pins.
  0x80 is a op-code to set initial led_values and direction.
  */
  
  send_byte(0x80);       // must be a command,,,
  send_byte(gpio);
  
  // 0xfb.toString(2);
  // 0xFB = "11111011"  should be correct enables miso as only input
  send_byte(0xFB);        // this is the direction a bit mask?
}





int get_cdone()
{
  uint8_t data;
  send_byte(0x81);
  data = recv_byte();
  // ADBUS6 (GPIOL2)
  return (data & 0x40) != 0;
}

// [[Rcpp::export]]
int event(std::string init_swtch)
{
  
  const char *devstr = NULL;
  
  enum ftdi_interface ifnum = INTERFACE_B;
  
  int led_value;
  
  if (init_swtch == "on")
    led_value = 1;
  else if (init_swtch == "off")
    led_value = 0;
  
  
  // ---------------------------------------------------------
  // iNITIalize USB connection to FT2232H
  // ---------------------------------------------------------
  
  fprintf(stderr, "init..\n");
  
  ftdi_init(&ftdic);
  ftdi_set_interface(&ftdic, ifnum);
  
  if (devstr != NULL) {
    if (ftdi_usb_open_string(&ftdic, devstr)) {
      fprintf(stderr, "Can't find iCE FTDI USB device (device string %s).\n", devstr);
      error();
    }
  } else {
    if (ftdi_usb_open(&ftdic, 0x0403, 0x6010)) {
      fprintf(stderr, "Can't find iCE FTDI USB device (vedor_id 0x0403, device_id 0x6010).\n");
      error();
    }
  }
  
  ftdic_open = true;
  
  if (ftdi_usb_reset(&ftdic)) {
    fprintf(stderr, "Failed to reset iCE FTDI USB device.\n");
    error();
  }
  
  if (ftdi_usb_purge_buffers(&ftdic)) {
    fprintf(stderr, "Failed to purge buffers on iCE FTDI USB device.\n");
    error();
  }
  
  if (ftdi_set_bitmode(&ftdic, 0xff, BITMODE_MPSSE) < 0) {
    fprintf(stderr, "Failed set BITMODE_MPSSE on iCE FTDI USB device.\n");
    error();
  }
  
  
  
  // enable clock divide by 5
  send_byte(0x8b);
  
  // set 6 MHz clock
  send_byte(0x86);
  send_byte(0x02);   // 1 or 2 MHz ?
  send_byte(0x00);
  
  fprintf(stderr, "cdone: %s\n", get_cdone() ? "high" : "low");
  
  // initialize data out
  set_gpio(1, 1, 1);
  
  fprintf(stderr, "cdone: %s\n", get_cdone() ? "high" : "low");
  
  
  
  uint8_t data[1]; 
  
  while(true) {
    // assert CS 
    
    fprintf(stderr, "writing data led_value %d\n", led_value);
    led_value = !led_value;
    
    set_gpio(0, 1, 1);
    
    if(led_value == 1)
      data[0] = 0xcc;
    else if(led_value == 0)
      data[0] = 0xdd;
    
    // write
    send_spi( data, 1 );
    
    // deassert CS
    set_gpio(1, 1, 1);
    
    fprintf(stderr, "done\n");
    sleep(1);
  }
  
  
  // ---------------------------------------------------------
  // Exit
  // ---------------------------------------------------------
  
  fprintf(stderr, "Bye.\n");
  ftdi_disable_bitbang(&ftdic);
  ftdi_usb_close(&ftdic);
  ftdi_deinit(&ftdic);
  return 0;
}