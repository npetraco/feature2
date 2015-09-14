#--------------------------------------------
#' @title ccf.2d
#' @description Wrapper for the CCF_2D C++/openCV call
#' 
#' @details XXXX
#'
#' @param XXXX
#' 
#' @return XXXX
#' 
#' @references XXXX
#'
#' @examples XXXX
#--------------------------------------------
ccf.2d <- function(dmatA, dmatB, x.maxlag, y.maxlag, bit.depth) {
  
  M <- nrow(dmatA)
  N <- ncol(dmatA)
  
  P <- nrow(dmatB)
  Q <- ncol(dmatB)
  
  #Since matchTemplate assumes the template (dmatB) fits inside the query (dmatA) we may need to do some
  #padding to dmatA
  #
  #Case 1 M>P & N>Q. Assumed by matchTemplate
  #
  #Case 2
#   if((M>P) & (N<Q)) {
#     
#   }

#CAN WE DO THIS ALL FROM THE C++ SIDE?????

}