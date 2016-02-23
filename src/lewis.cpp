// [[Rcpp::depends("RcppArmadillo")]]
#include <RcppArmadillo.h>
#include <opencv2/opencv.hpp>
#include "internals.hpp"

using namespace Rcpp;
using namespace cv;
using namespace std;

//---------------------------------------------------------------------
// Lewis's routine to compute s and s^2 "running sums" (look-up tables)
// Matrix I/O in column major format
//
// ff is the "flattened" f(u,v) in column major order
//
// nz_idxs are the indices of the LUTs that are not by definition = 0.
// If we know where they are we don't have to use if's in the for loop
// to find them.
// They are assumed to be in 0-index (offset indexing) format.
// Is there a way to just compute them instead of using expand.grid and
// which on the R side??
//
//---------------------------------------------------------------------
// [[Rcpp::export]]
NumericMatrix lewis(int num_svals, IntegerVector nz_idxs, NumericVector ff, int offset1, int offset2, int offset3) {
  
  //s LUT goes in col 0, s^2 LUT goes in col 1:
  NumericMatrix svals(num_svals,2);

  for(int i = 0; i<nz_idxs.length(); i++) {
    
    int sidx1 = nz_idxs(i) - offset1;
    int sidx2 = nz_idxs(i) - offset2;
    int sidx3 = nz_idxs(i) - offset3;
    
    // s(u,v) = f(u,v) + s(u-1,v) + s(u,v-1) - s(u-1,v-1):
    svals(nz_idxs(i),0) = ff(i) + svals(sidx1,0) + svals(sidx2,0) - svals(sidx3,0);
    
    // s^2(u,v) = f^2(u,v) + s^2(u-1,v) + s^2(u,v-1) - s^2(u-1,v-1):
    svals(nz_idxs(i),1) = pow(ff(i),2) + svals(sidx1,1) + svals(sidx2,1) - svals(sidx3,1);
  }

  return svals;
  
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
// [[Rcpp::export]]
void numerator_trick(NumericMatrix A, NumericMatrix B) {
    
  arma::mat aA(A.begin(), A.nrow(), A.ncol(), false);
  arma::mat aB(B.begin(), B.nrow(), B.ncol(), false);
  
  arma::mat numerator_mat = arma::conv2(aA,arma::fliplr(arma::flipud((aB))));
  arma::cx_mat res = arma::ifft2(arma::fft2(aA) % arma::fft2(arma::fliplr(arma::flipud(aB))));
  res.print();
  numerator_mat.print();
    
  //return numerator_mat;
  
}