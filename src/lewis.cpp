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
NumericMatrix lewis3(IntegerVector u_idxs, IntegerVector v_idxs, IntegerVector nz_idxs, NumericVector ff, 
            int offset1, int offset2, int offset3) {
  
  NumericMatrix svals(u_idxs.length(),2);

  for(int i = 0; i<nz_idxs.length(); i++) {
    
    int sidx1 = nz_idxs(i) - offset1;
    int sidx2 = nz_idxs(i) - offset2;
    int sidx3 = nz_idxs(i) - offset3;
    
    svals(nz_idxs(i),0) = ff(i) + svals(sidx1,0) + svals(sidx2,0) - svals(sidx3,0);
    svals(nz_idxs(i),1) = pow(ff(i),2) + svals(sidx1,1) + svals(sidx2,1) - svals(sidx3,1);
  }

  return svals;
  
}