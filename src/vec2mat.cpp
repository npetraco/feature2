// [[Rcpp::depends("RcppArmadillo")]]
#include <RcppArmadillo.h>

using namespace Rcpp;
using namespace std;

// [[Rcpp::export]]
NumericMatrix vec2mat(NumericVector row_idxs, NumericVector col_idxs, NumericVector mat_elems, int num_rows, int num_cols) {
  
  arma::mat dmat = arma::mat(num_rows, num_cols).fill(arma::datum::nan);
  
  //Rcout << dmat.size() << endl;
  //Rcout << dmat.n_rows << endl;
  //Rcout << dmat.n_cols << endl;
  //Rcout << mat_elems.length() << endl;
  
  for(int i = 0; i<mat_elems.length(); i++) {
    //Rcout << "i: " << i << " row-1: " << row_idxs(i)-1 << " col-1: " << col_idxs(i)-1 << " elem: " <<  mat_elems(i) << endl;
    //dmat.at(row_idxs[i]-1, col_idxs[i]-1 ) = mat_elems[i];  //No index checks
    dmat(row_idxs(i)-1, col_idxs(i)-1) = mat_elems(i);    //Index checks, slower but avoids segfaults when func is misused.
  }
  
  return wrap(dmat);
  
}