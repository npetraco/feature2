#--------------------------------------------
#' @title crop.circular.roi
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
crop.circular.roi <- function(dmat, xc.idx, yc.idx, pix.radius, crop.empty.border=TRUE, roi.type="inner"){
  
  #Make mask for circle where you want it:
  mask <- Circular_Mask_ROI(dmat, xc.idx, yc.idx, pix.radius)
  
  cropped.dmat <- array(NaN, dim(dmat))
  
  #Keep the inner part
  if(roi.type=="inner") {
    keep.idxs <- which(mask == 1, arr.ind=TRUE)    
  } 
  
  #keep what's outside the circle
  if(roi.type=="outer") {
    keep.idxs <- which(mask == 0, arr.ind=TRUE)    
  } 
  
  cropped.dmat[keep.idxs] <- dmat[keep.idxs]
  
  if(crop.empty.border==TRUE) {
    cropped.dmat <- crop.retangular.roi(cropped.dmat, clip.empty.border=TRUE)
  }
    
  return(cropped.dmat)

}