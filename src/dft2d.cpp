#include <Rcpp.h>
#include <opencv2/opencv.hpp>
#include "internals.hpp"

using namespace Rcpp;
using namespace cv;
using namespace std;

// [[Rcpp::export]]
NumericMatrix DFTMag2D(NumericMatrix dmat, std::string bitdepth) {
  
  //Copy input matrix dmat to an openCV container and scale/cast to required bit depth representation.
  Mat odmat;
  NumericMatrix2openCVMat(dmat, odmat);
  scale64ToDepth(odmat, bitdepth);
  
  //expand input image to optimal size with zero padding
  Mat padded;
  int m = getOptimalDFTSize(odmat.rows);
  int n = getOptimalDFTSize(odmat.cols);
  copyMakeBorder(odmat, padded, 0, m - odmat.rows, 0, n - odmat.cols, BORDER_CONSTANT, Scalar::all(0));
  
  Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
  
  //Zero out any NaNs in 32 bit float or 64 bit Mats. There was no great function for this in openCV
  if(bitdepth == "32bitfloat") {
    replaceNaNs<float>(odmat, 0.0);
  } else if (bitdepth == "64bit") {
    replaceNaNs<double>(odmat, 0.0);
  }
  
  Mat complexI;
  merge(planes, 2, complexI);         // Add to the expanded another plane with zeros

  dft(complexI, complexI);            // this way the result may fit in the source matrix
  
  // compute the magnitude and switch to logarithmic scale
  // => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
  split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
  magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
  Mat magI = planes[0];
  
  magI += Scalar::all(1);                    // switch to logarithmic scale
  log(magI, magI);

  // crop the spectrum, if it has an odd number of rows or columns
  magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

  // rearrange the quadrants of Fourier image  so that the origin is at the image center
  int cx = magI.cols/2;
  int cy = magI.rows/2;

  Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
  Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
  Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
  Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

  Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
  q0.copyTo(tmp);
  q3.copyTo(q0);
  tmp.copyTo(q3);

  q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
  q2.copyTo(q1);
  tmp.copyTo(q2);

  normalize(magI, magI, 0, 1, CV_MINMAX); // Transform the matrix with float values into a
                                          // viewable image form (float between values 0 and 1).

  NumericMatrix r_magI = openCVMat2NumericMatrix(magI);
  
  return r_magI;
  
}