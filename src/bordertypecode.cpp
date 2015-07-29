#include <Rcpp.h>
#include <opencv2/opencv.hpp>
#include "internals.hpp"

using namespace Rcpp;
using namespace cv;
using namespace std;

// [[Rcpp::export]]
int borderTypeCode(std::string borderType) {
  
  int code = -1;
   
  if(borderType=="constant") {
    code = cv::BORDER_CONSTANT;
  } else if(borderType=="replicate") {
    code = cv::BORDER_REPLICATE;
  } else if(borderType=="reflect") {
    code = cv::BORDER_REFLECT;
  } else if(borderType=="wrap") {
    code = cv::BORDER_WRAP;
  } else if(borderType=="reflect_101") {
    code = cv::BORDER_REFLECT_101;
  } else if(borderType=="transparent") {
    code = cv::BORDER_TRANSPARENT;
  } else if(borderType=="default") {
    code = cv::BORDER_DEFAULT;
  } else if(borderType=="isolated") {
    code = cv::BORDER_ISOLATED;
  } else {
    Rcpp::stop("Invalid. Your choices are: constant, replicate, reflect, wrap, reflect_101, transparent, default, isolated");
  }
  
  return code;
  
}
  