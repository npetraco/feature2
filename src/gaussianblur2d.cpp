#include <Rcpp.h>
#include <opencv2/opencv.hpp>
#include "internals.hpp"

using namespace Rcpp;
using namespace cv;
using namespace std;

// [[Rcpp::export]]
NumericMatrix GaussianBlur2D(NumericMatrix dmat, std::string bitdepth, int num_rows_kernel, int num_cols_kernel, double sigmaX = 0.0, double sigmaY = 0.0, std::string borderType = "default") {
  
  //Copy input matrix dmat to an openCV container and scale/cast to required bit depth representation.
  Mat odmat;
  NumericMatrix2openCVMat(dmat, odmat);
  scale64ToDepth(odmat, bitdepth);
  
  if(bitdepth=="64bit") {
    replaceNaNs<double>(odmat, 0.0);
  }
  if(bitdepth=="32bitfloat") {
    replaceNaNs<float>(odmat, 0.0);
  }
  
  //GaussianBlur(InputArray src, OutputArray dst, Size ksize, double sigmaX, double sigmaY=0, int borderType=BORDER_DEFAULT )
  GaussianBlur(odmat, odmat, Size(num_cols_kernel, num_rows_kernel), sigmaX, sigmaY, borderTypeCode(borderType) );
  return openCVMat2NumericMatrix(odmat);
  
}