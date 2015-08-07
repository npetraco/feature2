#include <Rcpp.h>
#include <opencv2/opencv.hpp>
#include "internals.hpp"

using namespace Rcpp;
using namespace cv;
using namespace std;

// [[Rcpp::export]]
NumericMatrix Fill_Holes(NumericMatrix dmat, NumericMatrix mask) {
  
  Rcout << "Caution, MUST convert to 8-bit." << endl;
  
  Mat odmat;
  NumericMatrix2openCVMat(dmat, odmat);
  scale64ToDepth(odmat, "8bit");
  
  Mat omask;
  NumericMatrix2openCVMat(mask, omask);
  scale64ToDepth(omask, "8bit");
  
  Mat odmat_filled;
  inpaint(odmat, omask, odmat_filled, 20.0, INPAINT_NS);

  return openCVMat2NumericMatrix(odmat_filled);
  
}