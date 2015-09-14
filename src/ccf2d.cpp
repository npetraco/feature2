#include <Rcpp.h>
#include <opencv2/opencv.hpp>
#include "internals.hpp"

using namespace Rcpp;
using namespace cv;
using namespace std;

// [[Rcpp::export]]
NumericMatrix CCF_2D_v2(NumericMatrix dmat, NumericMatrix tmplte, int x_maxlag, int y_maxlag, std::string bitdepth) {
  
  int M = dmat.nrow();
  int N = dmat.ncol();
  int P = tmplte.nrow();
  int Q = tmplte.ncol();
  
  //Rcout << "Rows Querry: " << M << endl;
  //Rcout << "Cols Querry: " << N << endl;
  //Rcout << "Rows Tmplt:  " << P << endl;
  //Rcout << "Cols Tmplt:  " << Q << endl;

  Mat odmat;
  Mat otmplte;
  
  if((M >= P) && (N >= Q)) {
    
    //Case matchTemplate assumes.
    //Rcout << "Case 1" << endl;
    NumericMatrix2openCVMat(dmat, odmat);
    NumericMatrix2openCVMat(tmplte, otmplte);
    //Convert matrices to be correlated with desired bitdepth to their entries.
    //Has to be before 0 padding or padded 0's get rescaled ...
    //Does lower bit depth help to speed up corr calcs???????? CHECK!!!!!!!!!!
    scale64ToDepth(odmat, bitdepth);
    scale64ToDepth(otmplte, bitdepth);
    
  } else if((M >= P) && (N < Q)) {
    
    //Template has Q-N more columns than dmat. Pad dmat with Q-N columns to the right
    //Rcout << "Case 2" << endl;
    NumericMatrix2openCVMat(tmplte, otmplte);
    NumericMatrix2openCVMat(dmat, odmat);
    //Convert matrices to be correlated with desired bitdepth to their entries.
    //Has to be before 0 padding or padded 0's get rescaled ...
    //Does lower bit depth help to speed up corr calcs???????? CHECK!!!!!!!!!!
    scale64ToDepth(odmat, bitdepth);
    scale64ToDepth(otmplte, bitdepth);
    copyMakeBorder(odmat, odmat, 0, 0, 0, Q-N, cv::BORDER_CONSTANT, Scalar::all(0));
    
  } else if((M < P) && (N >= Q)) {
    
    //Template has P-M more rows than dmat. Pad dmat with P-M rows on the bottom
    //Rcout << "Case 3" << endl;
    NumericMatrix2openCVMat(tmplte, otmplte);
    NumericMatrix2openCVMat(dmat, odmat);
    //Convert matrices to be correlated with desired bitdepth to their entries.
    //Has to be before 0 padding or padded 0's get rescaled ...
    //Does lower bit depth help to speed up corr calcs???????? CHECK!!!!!!!!!!
    scale64ToDepth(odmat, bitdepth);
    scale64ToDepth(otmplte, bitdepth);
    copyMakeBorder(odmat, odmat, 0, P-M, 0, 0, cv::BORDER_CONSTANT, Scalar::all(0));
    
  } else if((M < P) && (N < Q)) {
    
    //Template has P-M and Q-N more rows/columns than dmat. Just switch who is going to be the template and who is the querry
    //Rcout << "Case 4. Switching querry/tmplte." << endl;
    NumericMatrix2openCVMat(dmat, otmplte);
    NumericMatrix2openCVMat(tmplte, odmat);
    //Convert matrices to be correlated with desired bitdepth to their entries.
    //Has to be before 0 padding or padded 0's get rescaled ...
    //Does lower bit depth help to speed up corr calcs???????? CHECK!!!!!!!!!!
    scale64ToDepth(odmat, bitdepth);
    scale64ToDepth(otmplte, bitdepth);

  }
  
  //Below assumes we are now converted to Case 1. ***********This all could be more efficient. EG do one border op in ifs above.
  M = odmat.rows;
  N = odmat.cols;
  P = otmplte.rows;
  Q = otmplte.cols;
  
  //Rcout << "Now Rows Querry: " << M << endl;
  //Rcout << "Now Cols Querry: " << N << endl;
  //Rcout << "Now Rows Tmplt:  " << P << endl;
  //Rcout << "Now Cols Tmplt:  " << Q << endl;
  
  int x_maxlag_left = -1;
  int x_maxlag_right = -1;
  int y_maxlag_up = -1;
  int y_maxlag_down = -1;
  
  //Below we assume we are lagging left/right and up/down by the same amount (i.e. symmetrically):
  if(x_maxlag > Q-1) { //On the left, back it up
    x_maxlag_left = Q-1;
  } else {
    x_maxlag_left = x_maxlag;
  }
  
  if(x_maxlag > N+Q-1) { //On the right, push it forward 
    x_maxlag_right = N+Q-1;
    Rcout << "Cutting right max lag back to "<< N+Q-1 << ". " << "Request of "<< x_maxlag << " exceeds num cols of querry." << endl;
  } else {
    if(x_maxlag > N-Q) {
     x_maxlag_right = x_maxlag - (N-Q); //Extra padding of query as template shifts right, past the edge.
    } else {
      x_maxlag_right = 0; //No extra padding will be required if template doesn't shift over the right edge.
    }
  }

  if(y_maxlag > P-1) { //Up
    y_maxlag_up = P-1;
  } else {
    y_maxlag_up = y_maxlag;
  }

  if(y_maxlag > M+P-1) { //Down
    y_maxlag_down = M+P-1;
    Rcout << "Cutting down max lag top to "<< M+P-1 << ". " << "Request of "<< y_maxlag << " exceeds num rows of querry." << endl;
  } else {
    //y_maxlag_down = y_maxlag;
    if(y_maxlag > M-P) {
     y_maxlag_down = y_maxlag - (M-P); //Extra padding of query as template shifts down, past the bottom edge.
    } else {
      y_maxlag_down = 0; //No extra padding will be required if template doesn't shift down past the bottom edge.
    }
  }

  //Do the final pad so all the requested lags will be computed
  copyMakeBorder(odmat, odmat, y_maxlag_up, y_maxlag_down, x_maxlag_left, x_maxlag_right, BORDER_CONSTANT, Scalar::all(0));
  
  //Recalculate the dims of the query now that all padding is done:
  M = odmat.rows;
  N = odmat.cols;
  
  //Rcout << "Final Rows Querry for req lags: "<< M << endl;
  //Rcout << "Final Cols Querry for req lags: "<< N << endl;
  
  // Create the result matrix
  int result_cols = N-Q+1;
  int result_rows = M-P+1;
  //Rcout << result_cols << endl;
  //Rcout << result_rows << endl;
  
  //CCF values will be sotred as 32bit floats. *********TEST MORE AND SEE IF THERE IS A SPEED ADVANTAGE**********
  Mat result;
  result.create( result_rows, result_cols, CV_32FC1 );
  //Rcout << "Result mat rows: " << result.rows << endl;
  //Rcout << "Result mat cols: " << result.cols << endl;
  
  //CCF function is matchTemplate with CV_TM_CCOEFF_NORMED option.
  //matchTemplate is pretty optimized in openCV so we will use it instead of our own ccf2d routene 
  matchTemplate( odmat, otmplte, result, CV_TM_CCOEFF_NORMED);

  //Store/spitout the best/worst matching lags.
  double minVal; double maxVal; Point minLoc; Point maxLoc;

  minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
  Rcout << minVal << " is lowest correlation at x,y-lag: " << minLoc << endl;
  Rcout << maxVal << " is highest correlation at x,y-lag: " << maxLoc << endl;
  
  return openCVMat2NumericMatrix(result);
//  return openCVMat2NumericMatrix(odmat);

}