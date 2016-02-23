#include <Rcpp.h>
#include <opencv2/opencv.hpp>
#include "internals.hpp"

using namespace Rcpp;
using namespace cv;
using namespace std;

// Rearrange the quadrants of Fourier or inverse Fourier matrix so that the origin is at the matrix center

// [[Rcpp::export]]
NumericMatrix Center2D(NumericMatrix dmat) {
  
  //Copy input matrix dmat to an openCV container and scale/cast to required bit depth representation.
  Mat magI;
  NumericMatrix2openCVMat(dmat, magI);
  //scale64ToDepth(odmat, bitdepth);
  
  // crop the spectrum, if it has an odd number of rows or columns
  magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

  // rearrange the quadrants of Fourier image  so that the origin is at the image center
  int cx = magI.cols/2;
  int cy = magI.rows/2;
  
  Rcout << "Center is row:" << cy << " column:" << cx << endl;

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

  NumericMatrix r_magI = openCVMat2NumericMatrix(magI);
  
  return r_magI;
  
}