#--------------------------------------------
#' @title form.fit
#' @description Fit a 2D polynomial form for a sirface data set.
#' 
#' @details Handy to subtract a form
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
form.fit<-function(dmat, deg, num.x.pts=NULL, num.slices=NULL, x.phys.max=NULL, y.phys.max=NULL) {
  
  #Downsample the surface points for faster fiting:
  if(is.null(num.x.pts)) {
    num.x.pts <- ncol(dmat)
  }
  if(is.null(num.slices)) {
    num.slices <- nrow(dmat)
  }
  
  tot.num.pts <- num.x.pts*num.slices
  
  print(paste("Total # points: ", tot.num.pts))
  
  #More than 500,000 pts? Ask to plot.
  if(tot.num.pts>=5e5){
    print("Lots of points. Sure you want to do this? (y/n)")
    ans <- readline()
    if(ans=="n") {
      print("OK. Won't try to plot.")
      return()
    }
  }
  
  decimated.surf.mat <- Resize(dmat, num.slices, num.x.pts)
  xaxis <- 1:ncol(decimated.surf.mat)
  yaxis <- 1:nrow(decimated.surf.mat)
  
  #If a physical axis is requested make it the same length as the decimated point axis
  if(!is.null(x.phys.max)) {
    xaxis.phys <- seq(0, x.phys.max, length.out=length(xaxis))
  }
  if(!is.null(y.phys.max)) {
    yaxis.phys <- seq(0, y.phys.max, length.out=length(yaxis))
  }
  
  if( (!is.null(x.phys.max)) & (!is.null(y.phys.max)) ) {
    coords<-cbind(expand.grid(X=xaxis.phys, Y=yaxis.phys), as.numeric(t(decimated.surf.mat)))
    x<-coords[,1]
    y<-coords[,2]
    z<-coords[,3]    
  } else {
    coords<-cbind(expand.grid(X=xaxis, Y=yaxis), as.numeric(t(decimated.surf.mat)))
    x<-coords[,1]
    y<-coords[,2]
    z<-coords[,3]        
  }
  
  #FIT HERE
  if(deg == 1) {
    fit <- lm(z ~ x + y, na.action=na.omit)
  } else if(deg==2) {
    fit <- nlsLM(z ~ a + b*x + c*y + d*x^2 + e*y^2 + f*x*y, start = list(a = rnorm(1), 
                                                                         b = rnorm(1), 
                                                                         c = rnorm(1), 
                                                                         d = rnorm(1), 
                                                                         e = rnorm(1), 
                                                                         f = rnorm(1)), trace=T)    
  } else {
    stop("Not implemented yet.")
  }
  
  
  return(fit)
  
}