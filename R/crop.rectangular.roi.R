#--------------------------------------------
#' @title crop.ractangular.roi
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
crop.retangular.roi <- function(dmat, left.idx.start=NULL, right.idx.start=NULL, top.idx.start=NULL, bot.idx.start=NULL, 
                                clip.empty.border=TRUE, roi.type="inner"){
  
  if(clip.empty.border==TRUE) {
    
    #Remove any Na/NaN boundry or lines through the surface.
    col.rm.idxs <- which(colSums(dmat, na.rm=T) == 0)
    row.rm.idxs <- which(rowSums(dmat, na.rm=T) == 0)
    
    #Don't try to clip a Na/NaN boundry if none exist. 
    cropped.dmat <- dmat
    if(length(col.rm.idxs) > 0) {
      cropped.dmat <- cropped.dmat[,-col.rm.idxs]
    }
    if(length(row.rm.idxs) > 0) {
      cropped.dmat <- cropped.dmat[-row.rm.idxs,]
    }
    
  }
  
  if( (!is.null(left.idx.start)) & (!is.null(right.idx.start)) & (!is.null(top.idx.start)) & (!is.null(bot.idx.start)) ){
    width  <- right.idx.start - left.idx.start + 1
    height <- bot.idx.start - top.idx.start + 1
    
    if(roi.type=="inner") {
      cropped.dmat <- Crop_Retangular_ROI(cropped.dmat, Xstart_index = left.idx.start, Ystart_index = top.idx.start, Width = width, Height = height)
    }
    
    if(roi.type=="outer") {
      cropped.dmat[top.idx.start:bot.idx.start, left.idx.start:right.idx.start] <- NaN
    }
    
  }
  
  return(cropped.dmat)

}