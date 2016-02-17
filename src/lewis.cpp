// [[Rcpp::depends("RcppArmadillo")]]
#include <RcppArmadillo.h>
#include <opencv2/opencv.hpp>
#include "internals.hpp"

using namespace Rcpp;
using namespace cv;
using namespace std;


//---------------------------------------------------------------------
//Lewis's routine to compute s and s^2 troublesome terms
//---------------------------------------------------------------------
// [[Rcpp::export]]
NumericVector lewis3(IntegerVector u_idxs, IntegerVector v_idxs, IntegerVector nz_idxs, NumericVector ff, 
            int offset1, int offset2, int offset3) {
  
  NumericVector s_vals(u_idxs.length());
  NumericVector s_sq_vals(u_idxs.length());

  for(int i = 0; i<nz_idxs.length(); i++) {
    
    int sidx1 = nz_idxs(i) - offset1;
    int sidx2 = nz_idxs(i) - offset2;
    int sidx3 = nz_idxs(i) - offset3;
    
    s_vals(nz_idxs(i)) = ff(i) + s_vals(sidx1) + s_vals(sidx2) - s_vals(sidx3);
    s_sq_vals(nz_idxs(i)) = pow(ff(i),2) + s_sq_vals(sidx1) + s_sq_vals(sidx2) - s_sq_vals(sidx3);
  }

  return s_vals;
  
}