#--------------------------------------------
#' @title rescale.z
#' @description Put the measured z-valued into a 8 or 16-bit dynamic range. 
#' 
#' @details Useful and sometimes necessary preprocessing step for analyzing surface data with openCV.
#'
#' @param dmat       A matrix of data from R
#' @param z.bitdepth Desired bit depth or a stated rangelength of graylevels. 
#' Specified bit-depths may be "8bit" or "16bit" for "unsigned" dynamic ranges of those bit depths. A range of 
#' graylevels is specified with a list whose first element is "custom.dynamic.range" and whoes second element 
#' is the number of graylevels required.
#' @param zero.maskQ If TRUE, the smallest value of the dynamic range (0) is not used, with the intension that it is held in reserve for NaNs/NAs in the matrix.
#' 
#' @return re-scaled dmat with matrix elements in the unsigned 8 or 16-bit range.
#' 
#' @references XXXX
#'
#' @examples XXXX
#--------------------------------------------
rescale.z <- function(dmat, z.bitdepth, zero.maskQ=FALSE) {
  
  if(is.list(z.bitdepth)) {
    if(z.bitdepth[[1]] == "custom.dynamic.range") {
      
      zmax <- z.bitdepth[[2]] +  z.bitdepth[[3]]
      
    } else {
      stop(paste("First element of list should be custom.dynamic.range"))
    }
  } else {
    
    if(z.bitdepth == 8 | z.bitdepth == 16) {
      
      zmax <- 2^z.bitdepth
      
    } else {
      
      stop(paste("Bit depth of:", z.bitdepth, "not supported."))
      
    }
    
  }
  
  
  if(zero.maskQ==TRUE) {
    #Reserve the label 0 for NaNs and NAs. IE, don't use 0. Labels only run from 1:zmax-1 instead of 0:zmax-1
    rescaled.dmat <- matrix(as.numeric(as.character(cut(as.numeric(dmat), 
                                                        right=F, 
                                                        breaks=seq(min(dmat,na.rm=T),max(dmat,na.rm=T),length=(zmax)), 
                                                        labels=1:(zmax-1), 
                                                        include.lowest = T))), 
                            nrow=nrow(dmat), ncol=ncol(dmat))
    
  } else {
    #Data spans the full dynamic range of the chosen bit depth.
    rescaled.dmat <- matrix(as.numeric(as.character(cut(as.numeric(dmat), 
                                                        right=F, 
                                                        breaks=seq(min(dmat,na.rm=T),max(dmat,na.rm=T),length=(zmax+1)), 
                                                        labels=0:(zmax-1), 
                                                        include.lowest = T))), 
                            nrow=nrow(dmat), ncol=ncol(dmat))
    
  }
  
  return(rescaled.dmat)
  
}