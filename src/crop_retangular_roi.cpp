#include <Rcpp.h>
#include <opencv2/opencv.hpp>
#include "internals.hpp"

using namespace Rcpp;
using namespace cv;
using namespace std;

// [[Rcpp::export]]
NumericMatrix Crop_Retangular_ROI(NumericMatrix dmat, unsigned int X, unsigned int Y, unsigned int Width, unsigned int Height) {
  
  Mat odmat;
  NumericMatrix2openCVMat(dmat, odmat);
  
  Mat cropped_odmat = odmat(Rect(X,Y,Width,Height));
  
  return openCVMat2NumericMatrix(cropped_odmat);
  
}