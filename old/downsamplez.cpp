#include <Rcpp.h>
#include <opencv2/opencv.hpp>

using namespace Rcpp;
using namespace std;

// [[Rcpp::export]]
NumericMatrix downsamplez(NumericMatrix dmat, double min, double max, int zbitdepth) {
    
    //Rcout << dmat.nrow() << endl;
    //Rcout << dmat.ncol() << endl;
    
    NumericMatrix downsamped_dmat(dmat.nrow(), dmat.ncol());
    
    double dz = max-min; //inialize dz
    
    //Bin widths of possible dynamic ranges:
    if(zbitdepth == 8) {           //8-bit

      dz = dz/255.0;
      
    } else if (zbitdepth == 16) { //16-bit
    
      dz = dz/65535.0;

    } else if (zbitdepth == 32) { //32-bit
    
      dz = dz/4294967295.0;
      
    } else {
      Rcout << "That bit depth is not supported." << endl;
    }
    
    for(int i=0; i<dmat.nrow(); i++) {
      for(int j=0; j<dmat.ncol(); j++) {
        //uint8_t sres = floor(dmat[i,j]/dz - min/dz);
        //Rcout << (int)sres << endl;
        downsamped_dmat(i,j) = floor(dmat[i,j]/dz - min/dz);
      }
    }

    //Rcout << dz << endl;
    //Rcout << min/dz - min/dz << endl;
    //Rcout << max/dz - min/dz << endl;
    //Rcout << sizeof(uint16_t) << endl;
    
    //cv::Mat B = cv::Mat_<std::complex<double> >(3, 3);
    //cout << B.depth() << ", " << B.channels() << endl;
    
    return downsamped_dmat;
    
}
