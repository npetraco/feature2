#include <Rcpp.h>
#include <exception>
#include <opencv2/opencv.hpp>

using namespace Rcpp;
using namespace cv;
using namespace std;

//Internal. Not exported to R. Intended for conversion of doubles matrix input from R into openCV Mat and vice versa.
//Want this to be a handy container converter for featue extraction routines.

//---------------------------------------------------------------------
//R -> openCV  Like an as. Try to minimize copying?? NOTE: Does not convert 
//NumericMatrix from 64bit. Do that yourself with convertTo on the Mat object
//---------------------------------------------------------------------
void NumericMatrix2openCVMat(NumericMatrix dmat, Mat &odmat) {
  
  //NumericMatrix stored row-wise, but openCV Mat stored column wise?? This seems to work combind with the transpose below.
  Mat otmp(Size(dmat.nrow(), dmat.ncol()), CV_64F, dmat.begin(), Mat::AUTO_STEP);
  
  odmat = otmp.t(); //COULD THERE BE A PROBLEM HERE????? WHY DO WE HAVE TO DO THIS????
  
}


//---------------------------------------------------------------------
//openCV -> R Like a wrap.
//---------------------------------------------------------------------
NumericMatrix openCVMat2NumericMatrix(Mat &odmat) {
    
  if(odmat.depth() != CV_64F) {
    odmat.convertTo(odmat, CV_64F);
  }
  
  //NumericMatrix stored row-wise, but openCV Mat stored column wise?? This seems to work.
  Mat todmat = odmat.t();
  
  //Copy?? data from openCV Mat container to R NumericMatrix.
  NumericMatrix dmat(odmat.rows, odmat.cols, (double*)todmat.datastart);
  
  return dmat;

}


//---------------------------------------------------------------------
//Scale 64bit values into the dynamic range of a chosen bit depth 
//and then cast the scaled values into that depth
//---------------------------------------------------------------------
void scale64ToDepth(Mat &H, std::string typ) {
  
  if(H.depth() != CV_64F) {
    abort();
  }
  
  double mind;
  double maxd;
  cv::Point min_loc, max_loc;
  cv::minMaxLoc(H, &mind, &maxd, &min_loc, &max_loc);
  
  //Rcout << min << endl;
  //Rcout << max << endl;
  //Rcout << min_loc << endl;
  //Rcout << max_loc << endl;
  
  //Decide what bit depth the data should be represented as, and scale the data accordingly, with respect to 
  //the dynamic range of the chosen bit depth.
  if(typ == "8bit") {
    
    H.convertTo(H, CV_8U, 255.0/(maxd-mind), -255.0*mind/(maxd-mind));
    //H = H.t();
    
  } else if(typ == "16bit") {
    
    H.convertTo(H, CV_16U, 65535.0/(maxd-mind), -65535.0*mind/(maxd-mind));
    
  } else if(typ == "32bitfloat") {
    
    H.convertTo(H, CV_32F, 1.0/(maxd-mind), -1.0*mind/(maxd-mind));
    
  } 

}

//---------------------------------------------------------------------------------
// [[Rcpp::export]]
void testscale(NumericMatrix dmat, std::string bitdepth, bool printQ=false) {
  double t = (double)getTickCount();
  
  Mat odmat;
  NumericMatrix2openCVMat(dmat, odmat);
  
  if(printQ==true) {
    Rcout << "Before Scaling:" << endl;
    Rcout << odmat << endl;
  }
 
  scale64ToDepth(odmat, bitdepth);
  
  if(printQ==true) {
    Rcout << endl;
    Rcout << "After Scaling:" << endl;
    Rcout << odmat << endl;
  }
  
  t = ((double)getTickCount() - t)/getTickFrequency();
  Rcout << "Times passed in seconds: " << t << endl;

}


//---------------------------------------------------------------------
//---------------------------------------------------------------------
// [[Rcpp::export]]
NumericMatrix testConvert(NumericMatrix dmat, bool printQ=false) {
  
  double t = (double)getTickCount();
  
  //Convert R to openCV
  Mat otmp;
  NumericMatrix2openCVMat(dmat, otmp);
  
  if(printQ==true) {
    Rcout << "Converted R to openCV:" << endl;
    Rcout << otmp << endl;
  }
  
  //Convert openCV back to R and return
  NumericMatrix new_dmat = openCVMat2NumericMatrix(otmp);
  
  if(printQ==true) {
    Rcout << "Converted openCV back to R:" << endl;
    Rcout << endl;
  }
  
  t = ((double)getTickCount() - t)/getTickFrequency();
  Rcout << "Times passed in seconds: " << t << endl;
 
  return new_dmat;
  
}

//---------------------------------------------------------------------
//Replace NaNs with something in 64 and 32bitfloat Mats
//---------------------------------------------------------------------
template <typename T> 
void replaceNaNs(Mat &M, T substitute) {

  int nRows = M.rows;
  int nCols = M.cols;

  if (M.isContinuous())
  {
      nCols *= nRows;
      nRows = 1;
  }
  int i,j;
  //float* p;
  T* p;
  for( i = 0; i < nRows; ++i) {
    //p = M.ptr<float>(i);
    p = M.ptr<T>(i);
    for ( j = 0; j < nCols; ++j) {
      if(isnan(p[j])){
        p[j] = substitute; 
      }
    }
  }

}

template void replaceNaNs<float>(cv::Mat &M, float substitute); //Explicit instantiation for float
template void replaceNaNs<double>(cv::Mat &M, double substitute); //Explicit instantiation for double