#--------------------------------------------
#' @title form.fit
#' @description Fit a 2D linear or polynomial form for a surface data set.
#' 
#' @details Handy to subtract a form
#'
#' @param dmat        A matrix of points.
#' @param deg         Degree of surface to fit. 1-XXXXXX.
#' @param drop.val    Value of martix element in dmat to drop. Usually NaN or NA.
#' @param num.x.pts   Number of points desired in the x-direction.
#' @param num.slices  Number of points desired in the y-direction.
#' @param x.phys.max  For fitting to physical length/width.
#' @param y.phys.max  For fitting to physical length/width.
#' 
#' @return Matrix of points laying on the fit surface.
#' 
#' @references XXXX
#'
#' @examples XXXX
#--------------------------------------------
form.fit <- function(dmat, deg, drop.val="", num.x.pts=NULL, num.slices=NULL, x.phys.max=NULL, y.phys.max=NULL) {
  
  #Use dimensions of dmat if downsample params not provided
  if(is.null(num.x.pts)) {
    num.x.pts <- ncol(dmat)
  }
  if(is.null(num.slices)) {
    num.slices <- nrow(dmat)
  }
  
  tot.num.pts <- num.x.pts*num.slices
  
  print(paste("Total # points: ", tot.num.pts))
  
  #More than 5,000,000 pts? Ask before calc.
  if(tot.num.pts>=2e6){
    print("Lots of points. Sure you want to do this? (y/n)")
    ans <- readline()
    if(ans=="n") {
      print("OK. Won't try to do the fit.")
      return()
    }
  }
  
  #Downsample if requested:
  decimated.surf.mat <- Resize(dmat, num.slices, num.x.pts)
  
  coords <- mat2coords(decimated.surf.mat, x.phys.max, y.phys.max)
  
  #Don't use any zeroed/min/nan, etc coords. They were big holes.
  if(is.null(drop.val)) {
    
    print("Drop NULLs requested, but NULLS can't be elements!")
    
  } else if(is.number(drop.val)) {
    
    print(paste("Dropping all elements =",drop.val))
    coords <- coords[-which(coords[,3] == drop.val),]
    
  } else if(is.nan(drop.val)) {
    
    if(NaN %in% coords[,3]) {
      print("Dropping all NaN elements.")
      coords <- coords[-which(is.nan(coords[,3])) ,]      
    } else {
      print("Drop NaNs requested, but no NaNs to drop!")
    }
    
  } else if(is.na(drop.val)) {
    
    if(NA %in% coords[,3]) {
      print("Dropping all NA elements.")
      coords <- coords[-which(is.na(coords[,3])) ,]      
    } else {
      print("Drop NAs requested, but no NAs to drop!")
    }
    
  }
  
  if(!is.null(x.phys.max) & !is.null(y.phys.max) ) {
    x<-coords[,5] #Use physical x,y for fit
    y<-coords[,6]
    
  } else {
    x<-coords[,1] #Use index x,y for fit
    y<-coords[,2]    
  }
  z<-coords[,3]
  
  #FIT HERE
  if(deg == 1) {
    
    fit <- lm(z ~ x + y)
    #print(fit)
    #print(coef(fit))
    #pred <- predict(fit)
    coeffs <- coef(fit)
    pred <- x*coeffs[2] + y*coeffs[3] + coeffs[1]
    
    #Note: coords[,1] and coords[,2] are x, y indices, not physical x, y. We are just reformating the z to a rectangular matrix
    pred <- coords2mat(coords[,1], coords[,2], pred, x.max.idx=ncol(decimated.surf.mat), y.max.idx=nrow(decimated.surf.mat))
    
    #Scale back to original dimensions
    if( (nrow(decimated.surf.mat) != nrow(dmat)) & (ncol(decimated.surf.mat) != ncol(dmat))) {
      pred <- Resize(pred, nrow(dmat), ncol(dmat)) 
    }
    
  } else if(deg==2) {
    
    fit <- nlsLM(z ~ a + b*x + c*y + d*x^2 + e*y^2 + f*x*y, start = list(a = rnorm(1), 
                                                                         b = rnorm(1), 
                                                                         c = rnorm(1), 
                                                                         d = rnorm(1), 
                                                                         e = rnorm(1), 
                                                                         f = rnorm(1)), trace=T)

    pred <- predict(fit)
    
    #Note: coords[,1] and coords[,2] are x, y indices, not physical x, y  We are just reformating the z to a rectangular matrix
    pred <- coords2mat(coords[,1], coords[,2], pred, x.max.idx=ncol(decimated.surf.mat), y.max.idx=nrow(decimated.surf.mat))
    
    #Scale back to original dimensions
    if( (nrow(decimated.surf.mat) != nrow(dmat)) & (ncol(decimated.surf.mat) != ncol(dmat))) {
      pred <- Resize(pred, nrow(dmat), ncol(dmat)) 
    }
    
  } else if(deg==3) {
    
    fit <- nlsLM(z ~ a + b*x + c*y + d*x^2 + e*y^2 + f*x*y + g*x^3 + h*y^3 + ii*x^2*y + jj*x*y^2,
                    start = list(a = rnorm(1), 
                                 b = rnorm(1), 
                                 c = rnorm(1), 
                                 d = rnorm(1), 
                                 e = rnorm(1), 
                                 f = rnorm(1),
                                 g = rnorm(1),
                                 h = rnorm(1),
                                ii = rnorm(1),
                                jj = rnorm(1)),
                                 trace=T)
    
    pred <- predict(fit)
    
    #Note: coords[,1] and coords[,2] are x, y indices, not physical x, y  We are just reformating the z to a rectangular matrix
    pred <- coords2mat(coords[,1], coords[,2], pred, x.max.idx=ncol(decimated.surf.mat), y.max.idx=nrow(decimated.surf.mat))
    
    #Scale back to original dimensions
    if( (nrow(decimated.surf.mat) != nrow(dmat)) & (ncol(decimated.surf.mat) != ncol(dmat))) {
      pred <- Resize(pred, nrow(dmat), ncol(dmat)) 
    }
    
  } else if(deg==4){
    
    fit <- nlsLM(z ~ a + b*x + c*y + d*x^2 + e*y^2 + f*x*y + g*x^3 + h*y^3 + ii*x^2*y + jj*x*y^2 + k*x^4 + l*y^4 + m*x^3*y + n*x^2*y^2 + oo*x*y^3,
                 start = list(a = rnorm(1), 
                              b = rnorm(1), 
                              c = rnorm(1), 
                              d = rnorm(1), 
                              e = rnorm(1), 
                              f = rnorm(1),
                              g = rnorm(1),
                              h = rnorm(1),
                              ii = rnorm(1),
                              jj = rnorm(1),
                              k = rnorm(1),
                              l = rnorm(1),
                              m = rnorm(1),
                              n = rnorm(1),
                              oo = rnorm(1)),
                 trace=T)
    
    pred <- predict(fit)
    
    #Note: coords[,1] and coords[,2] are x, y indices, not physical x, y  We are just reformating the z to a rectangular matrix
    pred <- coords2mat(coords[,1], coords[,2], pred, x.max.idx=ncol(decimated.surf.mat), y.max.idx=nrow(decimated.surf.mat))
    
    #Scale back to original dimensions
    if( (nrow(decimated.surf.mat) != nrow(dmat)) & (ncol(decimated.surf.mat) != ncol(dmat))) {
      pred <- Resize(pred, nrow(dmat), ncol(dmat)) 
    }
    
  } else if(deg==5){
    
    fit <- nlsLM(z ~ a + b*x + c*y + d*x^2 + e*y^2 + f*x*y + g*x^3 + h*y^3 + ii*x^2*y + jj*x*y^2 + k*x^4 + l*y^4 + m*x^3*y + n*x^2*y^2 + oo*x*y^3 +
                   pp*x^5 + qq*y^5 + r*x^4*y^1 + s*x^3*y^2 + tt*x^2*y^3 + u*x*y^4,
                 start = list(a = rnorm(1), 
                              b = rnorm(1), 
                              c = rnorm(1), 
                              d = rnorm(1), 
                              e = rnorm(1), 
                              f = rnorm(1),
                              g = rnorm(1),
                              h = rnorm(1),
                              ii = rnorm(1),
                              jj = rnorm(1),
                              k = rnorm(1),
                              l = rnorm(1),
                              m = rnorm(1),
                              n = rnorm(1),
                              oo = rnorm(1),
                              pp = rnorm(1),
                              qq = rnorm(1),
                              r = rnorm(1),
                              s = rnorm(1),
                              tt = rnorm(1),
                              u = rnorm(1)),
                 trace=T)
    
    pred <- predict(fit)
    
    #Note: coords[,1] and coords[,2] are x, y indices, not physical x, y  We are just reformating the z to a rectangular matrix
    pred <- coords2mat(coords[,1], coords[,2], pred, x.max.idx=ncol(decimated.surf.mat), y.max.idx=nrow(decimated.surf.mat))
    
    #Scale back to original dimensions
    if( (nrow(decimated.surf.mat) != nrow(dmat)) & (ncol(decimated.surf.mat) != ncol(dmat))) {
      pred <- Resize(pred, nrow(dmat), ncol(dmat)) 
    }
    
  } else {
    stop("deg > 5 not implemented! Why would you want to go that high??")
  }

  return(pred)
  
}