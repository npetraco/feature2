#--------------------------------------------
#' @title hole.mask
#' @description XXXX
#' 
#' @details Holes are 1. Measured points are 0.
#'
#' @param XXXX
#' 
#' @return XXXX
#' 
#' @references XXXX
#'
#' @examples XXXX
#--------------------------------------------
hole.mask <- function(dmat) {
  
  mask <- array(0, c(nrow(dmat), ncol(dmat)))
  mask[which(is.nan(dmat) == TRUE, arr.ind = TRUE)] <- 1
  
  return(mask)
  
}