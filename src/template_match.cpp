#include <Rcpp.h>
#include <opencv2/opencv.hpp>
#include "internals.hpp"

using namespace Rcpp;
using namespace cv;
using namespace std;

// [[Rcpp::export]]
NumericMatrix TemplateMatch(NumericMatrix dmat_fixed, NumericMatrix template_mov, std::string bitdepth) {
  
  //Put matrix into openCV form
  Mat odmat;
  NumericMatrix2openCVMat(dmat_fixed, odmat);
  
  Mat otmpl;
  NumericMatrix2openCVMat(template_mov, otmpl);

  scale64ToDepth(odmat, bitdepth);
  scale64ToDepth(otmpl, bitdepth);
  
  // Create the result matrix
  //Below assumes we are now converted to Case 1.
  int My = odmat.rows;
  int Mx = odmat.cols;
  int Ny = otmpl.rows;
  int Nx = otmpl.cols;

  int result_cols = My-Ny+1;
  int result_rows = Mx-Nx+1;
  //Rcout << result_cols << endl;
  //Rcout << result_rows << endl;
  
  //CCF values will be sotred as 32bit floats. *********TEST MORE AND SEE IF THERE IS A SPEED ADVANTAGE**********
  Mat oresult;
  oresult.create( result_rows, result_cols, CV_32FC1 );
  //Rcout << "Result mat rows: " << oresult.rows << endl;
  //Rcout << "Result mat cols: " << oresult.cols << endl;
  
  
  //CCF function is matchTemplate with CV_TM_CCOEFF_NORMED option.
  //matchTemplate is pretty optimized in openCV so we will use it instead of our own ccf2d routene 
  matchTemplate( odmat, otmpl, oresult, CV_TM_CCOEFF_NORMED);
  
  //Store/spitout the best/worst matching lags.
  double minVal; double maxVal; Point minLoc; Point maxLoc;

  minMaxLoc( oresult, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
  //Rcout << minVal << " is lowest correlation at x,y-lag: " << minLoc << endl;
  Rcout << maxVal << " is highest correlation at x,y-lag: " << maxLoc << endl;
  Rcout << "Push template right " << maxLoc.x << " steps and down " << maxLoc.y << " steps." <<endl;
  
  NumericMatrix result(openCVMat2NumericMatrix(oresult));
  //colnames(result) = IntegerVector(seq(-(Nx-1),Mx-1));
  
  return result;
}