#--------------------------------------------
#' @title display2D_openCV
#' @description Wrapper for Display2D_openCV C++ function
#' 
#' @details Does some error checking and gets system file path to openCV executable in inst/display2d
#'
#' @param XXXX
#' 
#' @return XXXX
#' 
#' @references XXXX
#'
#' @examples XXXX
#--------------------------------------------
display2D.openCV <- function(dmat, num.rows, num.cols, bit.depth, file.ext, printQ=FALSE) {
  
  exe.path <- system.file("display2d", "./main",package="feature2")
  
  dmat.max <- max(dmat, na.rm=T)
  dmat.min <- min(dmat, na.rm=T)
  
  Display2D_openCV(dmat, num.rows, num.cols, bit.depth, file.ext, printQ, exe.path)
  
}