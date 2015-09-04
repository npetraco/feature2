#include <Rcpp.h>
#include <opencv2/opencv.hpp>
#include "internals.hpp"

using namespace Rcpp;
using namespace cv;
using namespace std;

// [[Rcpp::export]]
void CCF_Test1(NumericMatrix X, NumericMatrix H) {
  
  int M = X.nrow();
  int N = X.ncol();
  int P = H.nrow();
  int Q = H.ncol();
  
  NumericMatrix C(M+P-1, N+Q-1);
  
  int kbegin = 1-P;
  int kend = M-1;
  int lbegin = 1-Q;
  int lend = N-1;
  
  //Rcout << kbegin << endl;
  //Rcout << kend << endl;
  //Rcout << lbegin << endl;
  //Rcout << lend << endl;
  
  Rcout << "C #rows: " << M+P-1 << endl;
  Rcout << "C #cols: " << N+Q-1 << endl;
  
  for(int k = kbegin; k<=kend; k++) {
    for(int l = lbegin; l<=lend; l++) {
      
        //Rcout << "k: " << k << endl;
        //Rcout << "l: " << l << endl;
        //Rcout << "keff: " << k+P-1 << endl;
        //Rcout << "leff: " << l+Q-1 << endl;
        Rcout << "C(" << k+P-1 << "," << l+Q-1 << "): " << endl;
        
        for(int m=0; m<(M-1); m++) {
          for(int n=0; n<(N-1); n++) {
            Rcout << "    " << "X(" << m << "," << n << ")*H(" << m-k << "," << n-l << ")" << endl;
          }
        }
        
    }
  }
  
}