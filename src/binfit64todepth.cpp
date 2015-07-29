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
NumericMatrix binFit64ToDepth(NumericMatrix dmat, std::string bitdepth) {
  
  double mind = min(na_omit(dmat));
  double maxd = max(na_omit(dmat));
  
  //Rcout << mind << endl;
  //Rcout << maxd << endl;
  
  Mat odmat;
  NumericMatrix2openCVMat(dmat,odmat);
  scale64ToDepth(odmat, bitdepth);
  NumericMatrix tmp_dmat = openCVMat2NumericMatrix(odmat);
  
  arma::mat admat(tmp_dmat.begin(), tmp_dmat.nrow(), tmp_dmat.ncol(), false);
  
  double a = 0.0;
  double b = 0.0;
  
  if(bitdepth=="8bit") {
    a = 255.0/(maxd-mind);
    b = -255.0*mind/(maxd-mind);
  }
  if(bitdepth=="16bit") {
    a = 65535.0/(maxd-mind);
    b = -65535.0*mind/(maxd-mind);
  }
  if(bitdepth=="32bitfloat") {
    a = 1.0/(maxd-mind);
    b = -1.0*mind/(maxd-mind);
  }
  
  admat =  (admat - b)/a;
  
  return wrap(admat);

}