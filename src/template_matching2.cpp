#include <Rcpp.h>
#include <opencv2/opencv.hpp>
#include "internals.hpp"

using namespace Rcpp;
using namespace cv;
using namespace std;

// [[Rcpp::export]]
NumericMatrix TemplateMatchingTest2(NumericMatrix dmat, NumericMatrix tmplte) {
  
  //Copy input matrix dmat to an openCV container and scale/cast to required bit depth representation.
  Mat odmat;
  Mat otmplte;
  NumericMatrix2openCVMat(dmat, odmat);
  NumericMatrix2openCVMat(tmplte, otmplte);
  
  std::string bitdepth = "32bitfloat";
  scale64ToDepth(odmat, bitdepth);
  scale64ToDepth(otmplte, bitdepth);
  
  int M = odmat.rows;
  int N = odmat.cols;
//  m <- 1:M
//  n <- 1:N
//  
  int P = otmplte.rows;
  int Q = otmplte.cols;
//  p <- 1:P
//  q <- 1:Q
//  
//  Xt <- array(0.0, c(M+2*(P-1), N+2*(Q-1)))
//  Xt[m+P-1,n+Q-1] <- X   //PAD:
  Mat padded;
//  int m = getOptimalDFTSize(odmat.rows);
//  int n = getOptimalDFTSize(odmat.cols);
  //copyMakeBorder(odmat, padded, 0, (M+2*(P-1)) - odmat.rows, 0, (N+2*(Q-1)) - odmat.cols, BORDER_CONSTANT, Scalar::all(0));
  copyMakeBorder(odmat, padded, P-1, P-1, Q-1, Q-1, BORDER_CONSTANT, Scalar::all(0));
//
//
//  C <- array(0.0, c(M+P-1, N+Q-1))

  
  /// Create the result matrix
  int result_cols =  M+P-1;
  int result_rows = N+Q-1;
  
  Rcout << result_cols << endl;
  Rcout << result_rows << endl;
  
  Mat result;
  result.create( result_rows, result_cols, CV_32FC1 );
  Rcout << result.rows << endl;
  Rcout << result.cols << endl;
  
  /// Do the Matching and Normalize
  //matchTemplate( odmat, otmplte, result, CV_TM_CCORR_NORMED);
//  matchTemplate( odmat, otmplte, result, CV_TM_CCOEFF_NORMED);
  matchTemplate( padded, otmplte, result, CV_TM_CCOEFF_NORMED);
  //matchTemplate( odmat, otmplte, result, CV_TM_CCORR);
  //normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

  /// Localizing the best match with minMaxLoc
  double minVal; double maxVal; Point minLoc; Point maxLoc;
//  Point matchLoc;

  minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
  Rcout << minVal << endl;
  Rcout << maxVal << endl;
  Rcout << minLoc << endl;
  Rcout << maxLoc << endl;
  
  return openCVMat2NumericMatrix(result);
//  return openCVMat2NumericMatrix(padded);

}