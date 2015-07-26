#include <Rcpp.h>
#include <opencv2/opencv.hpp>
#include "internals.hpp"

using namespace Rcpp;
using namespace cv;
using namespace std;

// [[Rcpp::export]]
NumericMatrix Sobel(NumericMatrix dmat, std::string bitdepth, int ddepth, int dx, int dy, int ksize=3, double scale=1.0, double delta=0.0, int borderType=4) {
  
  //Copy input matrix dmat to an openCV container and scale/cast to required bit depth representation.
  Mat odmat;
  NumericMatrix2openCVMat(dmat, odmat);
  scale64ToDepth(odmat, bitdepth);
  
  // (Sobel) Gradient in a direction
  Mat grad_dir;
  //Rcout << BORDER_DEFAULT << endl;
  cv::Sobel( odmat, grad_dir, ddepth, dx, dy, ksize, scale, delta, borderType );

  //NumericMatrix r_grad_dir = openCVMat2NumericMatrix(grad_dir);
  
  return openCVMat2NumericMatrix(grad_dir);

}