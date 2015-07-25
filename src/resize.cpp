#include <Rcpp.h>
#include <opencv2/opencv.hpp>
#include "internals.hpp"

using namespace Rcpp;
using namespace cv;
using namespace std;

// [[Rcpp::export]]
NumericMatrix Resize(NumericMatrix dmat, uint num_rows, uint num_cols) {
  
  Mat odmat;
  NumericMatrix2openCVMat(dmat, odmat);
  
  Size size(num_cols, num_rows);
  Mat resized_odmat;
  resize(odmat, resized_odmat, size);

  NumericMatrix rdmat = openCVMat2NumericMatrix(resized_odmat);
  
  return rdmat;
  
}