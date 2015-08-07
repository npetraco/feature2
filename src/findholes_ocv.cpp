#include <Rcpp.h>
#include <opencv2/opencv.hpp>
#include "internals.hpp"

using namespace Rcpp;
using namespace cv;
using namespace std;

// [[Rcpp::export]]
NumericMatrix FindHoles(NumericMatrix dmat_mask, std::string mode, std::string approx_method, double big_hole_tol) {
  
  //Copy input matrix dmat to an openCV container and scale/cast to required bit depth representation.
  Mat odmat_mask;
  NumericMatrix2openCVMat(dmat_mask, odmat_mask);
  scale64ToDepth(odmat_mask, "8bit"); //Has to be 8-bit image
  
  vector<vector<Point> > contours; // Vector for storing contour
  vector<Vec4i> hierarchy;
  
  int mode_code = -1;
  if(mode == "external") {
    mode_code = CV_RETR_EXTERNAL;
  } else if(mode == "list") {
    mode_code = CV_RETR_LIST;
  } else if(mode == "ccomp") {
    mode_code = CV_RETR_CCOMP;
  } else if(mode == "tree") {
    mode_code = CV_RETR_TREE;
  }
  
  int method_code = -1;
  if(approx_method == "none") {
    method_code = CV_CHAIN_APPROX_NONE;
  } else if(approx_method == "simple") {
    method_code = CV_CHAIN_APPROX_SIMPLE;
  } else if(approx_method == "l1") {
    method_code = CV_CHAIN_APPROX_TC89_L1;
  } else if(approx_method == "kcos") {
    method_code = CV_CHAIN_APPROX_TC89_KCOS;
  }
  
  // Find the contours in the mask. They should corres
  findContours( odmat_mask, contours, hierarchy, mode_code, method_code );
  
  double total_area = odmat_mask.rows * odmat_mask.cols;
  Rcout << total_area << endl;
  
  Mat new_mask = odmat_mask.clone();
  
  for( int i = 0; i< contours.size(); i++ ) {
    double a = contourArea( contours[i],false);  //  Find the area of contour
    //Rcout << "Area of countour " << i << ":   " << a << endl;
    if( (a/total_area *100) > big_hole_tol) {
      //Rcout << contours[i] << endl;
      Rcout << "*******************Hole " << i << " Area percent: " << (a/total_area *100) << endl;
      Rcout << "Area of countour " << i << ":   " << a << endl;
      //drawContours(new_mask,contours,i,cvScalar(0),CV_FILLED,8,hierarchy,0,cv::Point());
      drawContours(new_mask, contours, i, Scalar(0), CV_FILLED);
    }
  }
  
//  /// Draw contours
//  RNG rng(12345);
//  Mat drawing = Mat::zeros( odmat.size(), CV_8U);
//  for( int i = 0; i< contours.size(); i++ ) {
//    Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
//    drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
//  }
//  
//  imwrite( fpath, drawing );
  return openCVMat2NumericMatrix(new_mask);
  
}