//#include <RcppCommon.h>
//#include <opencv2/opencv.hpp>
//
//namespace Rcpp {
//  //R -> C++ openCV Mat container   
//  template <> cv::Mat as(SEXP Rmat);
//  
//  //C++ openCV Mat container -> R
//  template<> SEXP wrap(const cv::Mat &M);
//}
//
//#include <Rcpp.h>
//
////Implementation
//namespace Rcpp {
//  template <> cv::Mat as(SEXP Rmatsexp) {
//  Rcpp::NumericMatrix Rmat(Rmatsexp);
//    
//  //Copy data from R into an openCV Mat container.
//  cv::Mat OCVmat(Rmat.nrow(), Rmat.ncol(), CV_64F);
//  for(int i = 0; i < OCVmat.rows; i++)
//    for(int j = 0; j < OCVmat.cols; j++)
//        OCVmat.at<double>(i,j)=Rmat(i,j);
//
//  return OCVmat;   
//  }
//}
void NumericMatrix2openCVMat(Rcpp::NumericMatrix dmat, cv::Mat &odmat);
Rcpp::NumericMatrix openCVMat2NumericMatrix(cv::Mat &odmat);
void scale64ToDepth(cv::Mat &M, std::string typ);
//void replaceNaNs(cv::Mat &M, float substitute);
//void replaceNaNs(cv::Mat &M, double substitute);
template <typename T> void replaceNaNs(cv::Mat &M, T substitute);