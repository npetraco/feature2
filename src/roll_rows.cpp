// [[Rcpp::depends("RcppArmadillo")]]
#include <RcppArmadillo.h>
#include <opencv2/opencv.hpp>
#include "internals.hpp"

using namespace Rcpp;
using namespace cv;
using namespace std;


//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
// [[Rcpp::export]]
NumericMatrix RollRows(NumericMatrix dmat, std::string direction, unsigned int n) {
  
  if(n == 0) {
    return dmat;
  }
  
  unsigned int num = n % dmat.nrow();
  
  arma::mat admat(dmat.begin(), dmat.nrow(), dmat.ncol(), false);
  
   arma::mat shifted_amat;
  
  if(direction == "up") {
    shifted_amat = join_cols(admat.tail_rows( dmat.nrow() - num ), admat.head_rows(num));
  }
  if(direction == "down") {
    shifted_amat = join_cols(admat.tail_rows(num), admat.head_rows(dmat.nrow() - num));
  }
  
  return wrap(shifted_amat);
  
}