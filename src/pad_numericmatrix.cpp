#include <Rcpp.h>
#include <opencv2/opencv.hpp>
#include "internals.hpp"

using namespace Rcpp;
using namespace cv;
using namespace std;

// [[Rcpp::export]]
NumericMatrix Pad_NumericMatrix(NumericMatrix dmat, int top, int bottom, int left, int right, std::string border_type, double value) {
  
  //Put matrix into openCV form
  Mat odmat;
  NumericMatrix2openCVMat(dmat, odmat);
  
  //Pad with value and requested type:
  Mat opadded;
  copyMakeBorder(odmat, opadded, top, bottom, left, right, borderTypeCode(border_type), value);
  
  return(openCVMat2NumericMatrix(opadded));
  
}