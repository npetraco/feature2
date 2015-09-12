#include <Rcpp.h>
#include <opencv2/opencv.hpp>
#include "internals.hpp"

using namespace Rcpp;
using namespace cv;
using namespace std;

// [[Rcpp::export]]
NumericMatrix CCF_2D(NumericMatrix dmat, NumericMatrix tmplte, int x_maxlag, int y_maxlag, std::string bitdepth) {
  
  //Copy input matrix dmat to an openCV container and scale/cast to required bit depth representation.
  Mat odmat;
  Mat otmplte;
  NumericMatrix2openCVMat(dmat, odmat);
  NumericMatrix2openCVMat(tmplte, otmplte);
  
  //Convert matrices to be correlated to desired bitdepth.
  //This can speed up correlation computations, but gives approx results. *********TEST MORE!!!!*******
  //std::string bitdepth = "32bitfloat";
  scale64ToDepth(odmat, bitdepth);
  scale64ToDepth(otmplte, bitdepth);
  
  int M = odmat.rows;
  int N = odmat.cols;
  int P = otmplte.rows;
  int Q = otmplte.cols;
  
  Mat padded;
  //******Add in machinery for variable maxlags in x and y. Right now, the entire template area is correlated to dmat.
  copyMakeBorder(odmat, padded, P-1, P-1, Q-1, Q-1, BORDER_CONSTANT, Scalar::all(0));
  
  // Create the result matrix
  int result_cols =  M+P-1;
  int result_rows = N+Q-1;
  
  //Rcout << result_cols << endl;
  //Rcout << result_rows << endl;
  
  //CCF values will be sotred as 32bit floats. *********TEST MORE AND SEE IF THERE IS A SPEED ADVANTAGE**********
  Mat result;
  result.create( result_rows, result_cols, CV_32FC1 );
  //Rcout << result.rows << endl;
  //Rcout << result.cols << endl;
  
  //CCF function is matchTemplate with CV_TM_CCOEFF_NORMED option.
  //matchTemplate is pretty optimized in openCV so we will use it instead of our own ccf2d routene 
  matchTemplate( padded, otmplte, result, CV_TM_CCOEFF_NORMED);

  //Store/spitout the best/worst matching lags.
  double minVal; double maxVal; Point minLoc; Point maxLoc;

  minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
  Rcout << minVal << " is lowest correlation at x,y-lag: " << minLoc << endl;
  Rcout << maxVal << " is highest correlation at x,y-lag: " << maxLoc << endl;
  
  return openCVMat2NumericMatrix(result);

}