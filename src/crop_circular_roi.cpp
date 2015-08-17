#include <Rcpp.h>
#include <opencv2/opencv.hpp>
#include "internals.hpp"

using namespace Rcpp;
using namespace cv;
using namespace std;

// [[Rcpp::export]]
NumericMatrix Circular_Mask_ROI(NumericMatrix dmat, int xc_idx, int yc_idx, int pix_radius) {
  
  Mat odmat;
  NumericMatrix2openCVMat(dmat, odmat);
  
  //Just make the mask
  //Black mask, same size as the image:
  Mat omask(odmat.size(), CV_8U, Scalar::all(0));

  //Draw in white filled circle:
  circle(omask, Point(xc_idx,yc_idx), pix_radius, Scalar::all(1), -1);
  
  return openCVMat2NumericMatrix(omask);

}