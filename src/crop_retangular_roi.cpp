#include <Rcpp.h>
#include <opencv2/opencv.hpp>
#include "internals.hpp"

using namespace Rcpp;
using namespace cv;
using namespace std;

// [[Rcpp::export]]
NumericMatrix Crop_Retangular_ROI(NumericMatrix dmat, unsigned int Xstart_index, unsigned int Ystart_index, unsigned int Width, unsigned int Height) {
  
  Mat odmat;
  NumericMatrix2openCVMat(dmat, odmat);
  
  Mat cropped_odmat = odmat(Rect(Xstart_index - 1, Ystart_index - 1, Width - 1, Height -1));
  return openCVMat2NumericMatrix(cropped_odmat);

}