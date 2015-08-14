#--------------------------------------------
#' @title mat2coords
#' @description Convert a surface or image matrix to xyz coordinates
#' 
#' @details Output x y coordinates can be either indices or physical.
#'
#' @param dmat        A matrix of points.
#' @param num.x.pts   Number of points desired in the x-direction.
#' @param num.slices  Number of points desired in the y-direction.
#' @param x.phys.max  XXXX
#' @param y.phys.max  XXXX
#' 
#' @return XXXX
#' 
#' @references XXXX
#'
#' @examples XXXX
#--------------------------------------------
mat2coords<-function(dmat, x.phys.max=NULL, y.phys.max=NULL) {
  
  xaxis <- 1:ncol(dmat)
  yaxis <- 1:nrow(dmat)
  
  #If a physical axis is requested make it the same length as the decimated point axis
  if( !is.null(x.phys.max) & !is.null(y.phys.max) ) {
    xaxis.phys <- seq(0, x.phys.max, length.out=length(xaxis))
    yaxis.phys <- seq(0, y.phys.max, length.out=length(yaxis))
  }
  
  if( (!is.null(x.phys.max)) & (!is.null(y.phys.max)) ) {
    
    #Will usually need the indecies of xaxis and yaxis too. Run on a separate processor
    xes <- cbind(xaxis, x.phys.max)
    yes <- cbind(yaxis, y.phys.max)
    registerDoMC(2)
    coords <- foreach(i=1:2, .combine="cbind") %dopar% {  
      cbind(expand.grid(X=xes[,i], Y=yes[,i]), as.numeric(t(dmat)))
    }
    
    coords <- cbind( coords[,c(1,2,6)], 1:nrow(coords), coords[,c(4,5)])
    colnames(coords) <- c("x.idx", "y.idx", "z", "row.idx", "x.phy", "y.phys")
  
  } else {
    
    coords<-cbind(expand.grid(X=xaxis, Y=yaxis), as.numeric(t(dmat)))
    coords <- cbind(coords,1:nrow(coords))
    colnames(coords) <- c("x.idx", "y.idx", "z", "row.idx")
    
  }
  
  return(coords)
  
}