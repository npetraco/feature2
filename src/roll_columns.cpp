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
NumericMatrix RollColumns(NumericMatrix dmat, std::string direction, unsigned int n) {
  
  if(n == 0) {
    return dmat;
  }
  
  unsigned int num = n % dmat.ncol();
  
  arma::mat admat(dmat.begin(), dmat.nrow(), dmat.ncol(), false);
  
   arma::mat shifted_amat;
   //arma::mat shifted_amat = join_rows(admat.tail_cols( dmat.ncol() - num ), admat.head_cols(num));
  
  if(direction == "forward") {
    shifted_amat = join_rows(admat.tail_cols(num), admat.head_cols(dmat.ncol() - num));
  }
  if(direction == "backward") {
    shifted_amat = join_rows(admat.tail_cols( dmat.ncol() - num ), admat.head_cols(num));
  }
  
  return wrap(shifted_amat);
  
}