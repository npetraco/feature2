#--------------------------------------------
#' @title plot3D.sip
#' @description 3D plot of a surface, image or points. 
#' 
#' @details 3D plot  of a surface image or points. Just send in XXXX. Some type of downsampling or decimation 
#' is usually required so that the surface can be rendered fast. This function leverages openCV functionality 
#' to do that. The The arguments num.x.pts (numcols) and num.slices (num.rows) allow the user to reduce the 
#' surface down to the desited size. If the number of points is still potentially large, the function will prompt the 
#' user if they want to continue.
#'
#' @param dmat        A matrix of points.
#' @param num.x.pts   Number of points desired in the x-direction.
#' @param num.slices  Number of points desired in the y-direction.
#' @param aspect      Aspect ratios
#' @param plot.type   "points" or "surface"
#' 
#' @return a 3D plot.
#' 
#' @references XXXX
#'
#' @examples XXXX
#--------------------------------------------
plot3D.sip<-function(dmat, num.x.pts=NULL, num.slices=NULL, aspect=c(1,0.3,0.2), plot.type="points") {
  
  surf.mat<-dmat
  
  #Downsample the surface points for faster plotting:
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
  
  decimated.surf.mat <- Resize(surf.mat, num.slices, num.x.pts)
  xaxis <- 1:ncol(decimated.surf.mat)
  yaxis <- 1:nrow(decimated.surf.mat)
  
  if(plot.type=="points"){
    
    coords<-cbind(expand.grid(X=xaxis, Y=yaxis), as.numeric(t(decimated.surf.mat)))
    x<-coords[,1]
    y<-coords[,2]
    z<-coords[,3]
    
    #Swap x and y axes to put origin in top left corner (image coordinates)
    rgl.plot.obj <- plot3d(y,x,z,radius=0.01, xlab="x",ylab="y",zlab="z",col="black",aspect=aspect,type="p")
    
    #return(list(rgl.plot.obj, coords))
    
  } else if(plot.type=="surface") {
    
    nbcol = 256
    color = rev(rainbow(nbcol, start = 0/6, end = 2/6)) #Color band width
    #zcol  = cut(t(decimated.surf.mat), nbcol)   
    zcol  = cut(decimated.surf.mat, nbcol)   
    #persp3d(xaxis, yaxis, t(decimated.surf.mat), aspect=aspect, col=color[zcol])
    
    #Swap x and y axes to put origin in top left corner (image coordinates)
    rgl.plot.obj <- persp3d(yaxis, xaxis, decimated.surf.mat, aspect=aspect, col=color[zcol])
    
    #coords<-cbind(expand.grid(X=xaxis, Y=yaxis), as.numeric(t(decimated.surf.mat)))
    #return(list(rgl.plot.obj, coords))
    
  } else {
    print("Pick surface or points.")
  }      
  
}