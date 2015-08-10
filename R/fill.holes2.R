#--------------------------------------------
#' @title fill.holes2
#' @description XXXX
#' 
#' @details Fill holes with openCV findContour and inPaint functionality. Back transform 8-bit result to approximate
#' 64-bit physical scale of original data.
#'
#' @param XXXX
#' 
#' @return XXXX
#' 
#' @references XXXX
#'
#' @examples XXXX
#--------------------------------------------
fill.holes2 <- function(dmat, big.hole.tol.percent) {
  
  holemask <- hole.mask(dmat, big.hole.tol.percent)

  #Scale the surface to 8-bit and fill holes (w/ 8-bit values)
  dmat.hole.filled <- Fill_Holes(dmat, holemask)
  
  #Re Scale filled holes back to original-ish scale
  filling <- approx.backscale.points(min(dmat,na.rm=T), max(dmat,na.rm=T), dmat.hole.filled[which(holemask==1,arr.ind = T)], "8bit")
  dmat.hole.filled <- dmat
  dmat.hole.filled[which(holemask==1,arr.ind = T)] <- filling
  
  return(dmat.hole.filled)
  
}