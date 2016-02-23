// [[Rcpp::depends("RcppArmadillo")]]
#include <RcppArmadillo.h>
#include <opencv2/opencv.hpp>
#include "internals.hpp"

using namespace Rcpp;
using namespace cv;
using namespace std;


//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
// [[Rcpp::export]]
arma::mat Reverse_Mat(NumericMatrix dmat) {
    
  arma::mat admat(dmat.begin(), dmat.nrow(), dmat.ncol(), false);
  //arma::mat admat = Rcpp::as<arma::mat>(dmat);
  
  arma::mat reversed_amat = fliplr(flipud(admat));
  //arma::mat reversed_amat(3,3);
   
  //reversed_amat.print();
    
  return reversed_amat;
  
}