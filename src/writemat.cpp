#include <Rcpp.h>
#include <opencv2/opencv.hpp>
#include "internals.hpp"

using namespace Rcpp;
using namespace cv;
using namespace std;

//Internal. Not exported to R. Intended for conversion of doubles matrix input from R into openCV Mat container
//featue extraction routines.

// [[Rcpp::export]]
void WriteMat(NumericMatrix dmat, std::string bitdepth, std::string fpath) {
  
  Mat odmat;
  NumericMatrix2openCVMat(dmat, odmat);
  if(bitdepth != "64bit") {
    scale64ToDepth(odmat, bitdepth);
  }
  
  imwrite( fpath, odmat );
  
}