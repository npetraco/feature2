#include <Rcpp.h>
#include <opencv2/opencv.hpp>
#include "internals.hpp"

using namespace Rcpp;
using namespace cv;
using namespace std;

// [[Rcpp::export]]
NumericMatrix Filter2D(NumericMatrix dmat, NumericMatrix kernel, std::string bitdepth) {
  
  //Copy?? input matrix dmat to an openCV container and scale/cast to required bit depth representation.
  Mat odmat;
  NumericMatrix2openCVMat(dmat, odmat);
  scale64ToDepth(odmat, bitdepth);
  
  //double mind;
  //double maxd;
  //cv::Point min_loc, max_loc;
  //cv::minMaxLoc(odmat, &mind, &maxd, &min_loc, &max_loc);
  //
  //Rcout << mind << endl;
  //Rcout << maxd << endl;
  //Rcout << min_loc << endl;
  //Rcout << max_loc << endl;


  //Don't want the scaling on the kernel elements. So convert kernel to 64bit Mat first then do a 
  //strait openCV convertTo:
  Mat okernel;
  NumericMatrix2openCVMat(kernel, okernel);
  //Rcout << okernel << endl;
  
  if(bitdepth == "8bit") {
    
    okernel.convertTo(okernel,CV_8S);
    
  } else if(bitdepth == "16bit") {
    
    okernel.convertTo(okernel,CV_16S);
    
  } else if (bitdepth == "32bitfloat") {
    
    okernel.convertTo(okernel,CV_32F);
    
  }
  
  //Rcout << okernel << endl;
  
  Mat filt_odmat;
  filter2D(odmat, filt_odmat, odmat.depth(), okernel);
  
  //Rcout << filt_odmat << endl;

  NumericMatrix r_filt_odmat = openCVMat2NumericMatrix(filt_odmat);
  //NumericMatrix r_filt_odmat;
  
  return r_filt_odmat;
  
}