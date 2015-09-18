#--------------------------------------------
#' @title coords2mat
#' @description Convert a vector of z's (heights, intensities, etc.) to a matrix
#' 
#' @details Convert a vector of z's (heights, intensities, etc.) to a matrix 
#'
#' @param XXXX
#' 
#' @return XXXX
#' 
#' @references XXXX
#'
#' @examples XXXX
#--------------------------------------------
coords2mat<-function(x.idxs, y.idxs, z.vec, x.max.idx=NULL, y.max.idx=NULL) {
  
  if(!is.null(x.max.idx) & !is.null(y.max.idx)) {
    
    num.cols <- x.max.idx
    num.rows <- y.max.idx
    dmat <- array(NaN, c(num.rows, num.cols))
    #print(dim(dmat))
  } else {
    
    num.cols <- max(x.idxs)
    num.rows <- max(y.idxs)
    dmat <- array(NaN, c(num.rows, num.cols))
    
  }
  
  #C++-ed for speed. R loop was pretty slow with big matrices
  dmat <- vec2mat(y.idxs, x.idxs, z.vec, num.rows, num.cols)

  return(dmat)
  
}
