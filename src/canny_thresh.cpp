#include <Rcpp.h>
#include <opencv2/opencv.hpp>
#include "internals.hpp"

using namespace Rcpp;
using namespace cv;
using namespace std;

// [[Rcpp::export]]
NumericMatrix Canny_Thresh(NumericMatrix dmat, std::string bitdepth, int lowThreshold, int ratio=3, int kernel_size=3) {
  
  //Copy input matrix dmat to an openCV container and scale/cast to required bit depth representation.
  Mat odmat;
  NumericMatrix2openCVMat(dmat, odmat);
  scale64ToDepth(odmat, bitdepth);
  
  Mat detected_edges;
  
  // Reduce noise with a kernel 3x3
  blur( odmat, detected_edges, Size(3,3) );

  // Canny detector
  Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );
  
  // Using Canny's output as a mask, we display our result
  Mat dst;
  dst.create( odmat.size(), odmat.type() );

  dst = Scalar::all(0);

  odmat.copyTo( dst, detected_edges);
  
  return openCVMat2NumericMatrix(dst);

}