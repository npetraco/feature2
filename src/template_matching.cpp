#include <Rcpp.h>
#include <opencv2/opencv.hpp>
#include "internals.hpp"

using namespace Rcpp;
using namespace cv;
using namespace std;

// [[Rcpp::export]]
NumericMatrix TemplateMatchingTest(NumericMatrix dmat, NumericMatrix tmplte) {
  
  //Copy input matrix dmat to an openCV container and scale/cast to required bit depth representation.
  Mat odmat;
  Mat otmplte;
  NumericMatrix2openCVMat(dmat, odmat);
  NumericMatrix2openCVMat(tmplte, otmplte);
  
  std::string bitdepth = "32bitfloat";
  scale64ToDepth(odmat, bitdepth);
  scale64ToDepth(otmplte, bitdepth);
  
  /// Create the result matrix
  int result_cols =  odmat.cols - otmplte.cols + 1;
  int result_rows = odmat.rows - otmplte.rows + 1;
  
  Rcout << result_cols << endl;
  Rcout << result_rows << endl;
  
  Mat result;
  result.create( result_rows, result_cols, CV_32FC1 );
  //Rcout << result.rows << endl;
  Rcout << result.cols << endl;
  
  /// Do the Matching and Normalize
  matchTemplate( odmat, otmplte, result, CV_TM_CCORR_NORMED);
  //matchTemplate( odmat, otmplte, result, CV_TM_CCORR);
  //normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

  /// Localizing the best match with minMaxLoc
  double minVal; double maxVal; Point minLoc; Point maxLoc;
  Point matchLoc;

  minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
  Rcout << minVal << endl;
  Rcout << maxVal << endl;
  Rcout << minLoc << endl;
  Rcout << maxLoc << endl;
  
  return openCVMat2NumericMatrix(result);

}