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

struct ftdi_context ftdic;
bool ftdic_open = false;
bool verbose = false;

// Below is from iceprog.c in Clifford Wolf's icestorm suite:
// https://github.com/cliffordwolf/icestorm

// [[Rcpp::export]]
void check_rx()
{
  while (1) {
    uint8_t data;
    int rc = ftdi_read_data(&ftdic, &data, 1);
    if (rc <= 0) break;
    fprintf(stderr, "unexpected rx byte: %02X\n", data);
  }
}

// [[Rcpp::export]]
void error()
{
  check_rx();
  fprintf(stderr, "ABORT.\n");
  if (ftdic_open)
    ftdi_usb_close(&ftdic);
  ftdi_deinit(&ftdic);
  exit(1);
}
