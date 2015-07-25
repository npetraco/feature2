#include <Rcpp.h>
#include <opencv2/opencv.hpp>

using namespace Rcpp;
using namespace cv;
using namespace std;

//Internal. Not exported to R. Intended for conversion of doubles matrix input from R into openCV Mat and vice versa.
//Want this to be a handy container converter for featue extraction routines.

//---------------------------------------------------------------------
//R -> openCV  Like an as.
//---------------------------------------------------------------------
Mat NumericMatrix2openCVMat(NumericMatrix dmat, std::string typ) {
    
  //Copy data from R into an openCV Mat container as a 64bit float.
  Mat H(dmat.nrow(), dmat.ncol(), CV_64F);
  for(int i = 0; i < H.rows; i++) {
    for(int j = 0; j < H.cols; j++) {
      H.at<double>(i,j)=dmat(i,j);
    }
  }
  
  double maxd = max(na_omit(dmat));
  double mind = min(na_omit(dmat));
  
  //Decide what bit depth the data should be represented as, and scale the data accordingly, with respect to 
  //the dynamic range of the chosen bit depth.
  if(typ == "8bit") {
    
    H.convertTo(H, CV_8U, 255.0/(maxd-mind), -255.0*mind/(maxd-mind));
    
  } else if(typ == "16bit") {
    
    H.convertTo(H, CV_16U, 65535.0/(maxd-mind), -65535.0*mind/(maxd-mind));
    
  } else if(typ == "32bitfloat") {
    
    H.convertTo(H, CV_32F, 1.0/(maxd-mind), -1.0*mind/(maxd-mind));
    
  } 
    
  return H; //Does this call clone or copyTo?? 
            //Doc says only Mat headers are copied (no data is copied) unless they are called.

}

//---------------------------------------------------------------------
//R -> openCV  Like an as. Try to minimize copying?? NOTE: Does not convert 
//NumericMatrix from 64bit. Do that yourself with convertTo
//---------------------------------------------------------------------
void NumericMatrix2openCVMat_2(NumericMatrix dmat, Mat &odmat) {
  
  Mat H(Size(dmat.ncol(), dmat.nrow()), CV_64F, dmat.begin(), Mat::AUTO_STEP);
  
  odmat = H;
  
}

//---------------------------------------------------------------------
//openCV -> R Like a wrap.
//---------------------------------------------------------------------
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


//---------------------------------------------------------------------
//openCV -> R Like a wrap.
//---------------------------------------------------------------------
NumericMatrix openCVMat2NumericMatrix_2(Mat &odmat) {
    
  if(odmat.depth() != CV_64F) {
    odmat.convertTo(odmat, CV_64F);
  }
    
  //Copy data from openCV Mat container to R NumericMatrix.
  NumericMatrix dmat(odmat.rows, odmat.cols, (double*)odmat.datastart);
  
  return dmat;

}


//---------------------------------------------------------------------
//---------------------------------------------------------------------
// [[Rcpp::export]]
NumericMatrix testConvert(NumericMatrix dmat, std::string typ) {
  
  double t = (double)getTickCount();
  
  //Convert R to openCV
  Mat otmp = NumericMatrix2openCVMat(dmat, typ);
  
  //Convert openCV back to R and return
  NumericMatrix new_dmat = openCVMat2NumericMatrix(otmp);
  
  t = ((double)getTickCount() - t)/getTickFrequency();
  Rcout << "Times passed in seconds: " << t << endl;
  
  return new_dmat;
}


//---------------------------------------------------------------------
//---------------------------------------------------------------------
// [[Rcpp::export]]
NumericMatrix testConvert_2(NumericMatrix dmat) {
  
  double t = (double)getTickCount();
  
  //Convert R to openCV
  Mat otmp;
  NumericMatrix2openCVMat_2(dmat, otmp);
  
  //Convert openCV back to R and return
  NumericMatrix new_dmat = openCVMat2NumericMatrix(otmp);
  
  t = ((double)getTickCount() - t)/getTickFrequency();
  Rcout << "Times passed in seconds: " << t << endl;
 
  return new_dmat;
  
}


//---------------------------------------------------------------------
//---------------------------------------------------------------------
// [[Rcpp::export]]
NumericMatrix testConvert_3(NumericMatrix dmat) {
  
  double t = (double)getTickCount();
  
  //Convert R to openCV
  Mat otmp;
  NumericMatrix2openCVMat_2(dmat, otmp);
  
  //Convert openCV back to R and return
  NumericMatrix new_dmat = openCVMat2NumericMatrix_2(otmp);
  
  t = ((double)getTickCount() - t)/getTickFrequency();
  Rcout << "Times passed in seconds: " << t << endl;
 
  return new_dmat;
  
}