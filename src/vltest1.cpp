#include <Rcpp.h>
extern "C" {
  #include <vl/generic.h>
}

using namespace Rcpp;
using namespace std;

// [[Rcpp::export]]
void vltest() {
  VL_PRINT ("Hello world!") ;
}