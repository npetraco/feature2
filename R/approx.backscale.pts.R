#--------------------------------------------
#' @title approx.backscale.points
#' @description XXXX
#' 
#' @details Approximately backscale bit depth scales values to wiah they were or would have been in the original 
#' 64 bit physical scale. Handy for when using the 8-bit only openCV hole filling routene.
#'
#' @param XXXX
#' 
#' @return XXXX
#' 
#' @references XXXX
#'
#' @examples XXXX
#--------------------------------------------
approx.backscale.points <- function(mind, maxd, pts.to.rescale, bitdepth) {
  
  if(bitdepth=="8bit") {
    a <- 255.0/(maxd-mind);
    b <- -255.0*mind/(maxd-mind);
  }
  if(bitdepth=="16bit") {
    a <- 65535.0/(maxd-mind);
    b <- -65535.0*mind/(maxd-mind);
  }
  if(bitdepth=="32bitfloat") {
    a <- 1.0/(maxd-mind);
    b <- -1.0*mind/(maxd-mind);
  }
  
  backscaled.pts =  (pts.to.rescale - b)/a;
  
  return(backscaled.pts)
  
}
  
  