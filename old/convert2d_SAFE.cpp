#include <Rcpp.h>
#include <opencv2/opencv.hpp>

using namespace Rcpp;
using namespace cv;
using namespace std;

//Internal. Not exported to R. Intended for conversion of doubles matrix input from R into openCV Mat and vice versa.
//Want this to be a handy container converter for featue extraction routines.

//R -> openCV  Like an as.
Mat NumericMatrix2openCVMat(NumericMatrix dmat, double maxd, double mind, std::string typ) {
    
  //Copy data from R into an openCV Mat container as a 64bit float.
  Mat H(dmat.nrow(), dmat.ncol(), CV_64F);
  for(int i = 0; i < H.rows; i++) {
    for(int j = 0; j < H.cols; j++) {
      H.at<double>(i,j)=dmat(i,j);
    }
  }
  
  //Decide what bit depth the data should be represented as.
  //***NOTE: The values in dmat should already be scaled to the 8 or 16-bit ranges!
  if(typ == "8bit") {
    
    H.convertTo(H, CV_8U, 255.0/(maxd-mind), -255.0*mind/(maxd-mind));
    
  } else if(typ == "16bit") {
    
    H.convertTo(H, CV_16U, 65535.0/(maxd-mind), -65535.0*mind/(maxd-mind));
    
  } else if(typ == "32bitfloat") {
    
    H.convertTo(H, CV_32F, 1.0/(maxd-mind), -1.0*mind/(maxd-mind));
    
  } 
    
  return H;

}

//openCV -> R Like a wrap.
NumericMatrix openCVMat2NumericMatrix(Mat odmat) {
  
  //Copy data from openCV Mat container to R NumericMatrix.
  NumericMatrix dmat(odmat.rows, odmat.cols);
  
  //PUT IN PROBE FOR TYPE. DONT DO IT ALREADY double
  odmat.convertTo(odmat, CV_64F);
  
  //Mat H(dmat.nrow(), dmat.ncol(), CV_64F);
  for(int i = 0; i < dmat.nrow(); i++) {
    for(int j = 0; j < dmat.ncol(); j++) {
      dmat(i,j) = odmat.at<double>(i,j);
    }
  }
  
  return dmat;

}

// [[Rcpp::export]]
NumericMatrix testConvert(NumericMatrix dmat, double maxd, double mind, std::string typ) {
  
  //Convert R to openCV
  Mat otmp = NumericMatrix2openCVMat(dmat, maxd, mind, typ);
  
  //Convert openCV back to R and return
  NumericMatrix new_dmat = openCVMat2NumericMatrix(otmp);
  
  return new_dmat;
}