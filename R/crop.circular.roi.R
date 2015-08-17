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
  
  
  mask <- Circular_Mask_ROI(dmat, xc.idx, yc.idx, pix.radius)
  if(roi.type=="inner") {
    
    cropped.dmat <- array(NaN, dim(dmat))
    keep.idxs <- which(mask == 1, arr.ind=TRUE)
    cropped.dmat[keep.idxs] <- dmat[keep.idxs]
    
    if(crop.empty.border==TRUE) {
      cropped.dmat <- crop.retangular.roi(cropped.dmat, clip.empty.border=TRUE)
    }
  }
  
#   if(roi.type=="outer") {
#     cropped.dmat[top.idx.start:bot.idx.start, left.idx.start:right.idx.start] <- NaN
#   }
  
  
  
  return(cropped.dmat)

}