cross.corr.2d.test <- function(X, H, normalizeQ=FALSE) {
  
  M <- nrow(X)
  N <- ncol(X)
  m <- 1:M
  n <- 1:N
  
  P <- nrow(H)
  Q <- ncol(H)
  p <- 1:P
  q <- 1:Q
  
  Xt <- array(0.0, c(M+2*(P-1), N+2*(Q-1)))
  Xt[m+P-1,n+Q-1] <- X
  
  C <- array(0.0, c(M+P-1, N+Q-1))
  
  if(normalizeQ==TRUE) {
    mH <- mean(H, na.rm=T)
  }
  
  for(k in 1:(M+P-1)) {
    for(l in 1:(N+Q-1)) {
      
      if(normalizeQ==TRUE) {
        
        mXt <- mean(Xt[p+k-1,q+l-1], na.rm=T)
        C[k,l] <- sum((Xt[p+k-1,q+l-1] - mXt) * (H-mH), na.rm=T) / sqrt(sum((Xt[p+k-1,q+l-1] - mXt)^2, na.rm=T) * sum((H-mH)^2, na.rm=T))
        
      } else {
        
        Hkl <- array(0.0, c(M+2*(P-1), N+2*(Q-1)))
        Hkl[p+k-1,q+l-1] <- H
        C[k,l] <- sum(Xt * Hkl, na.rm=T)
        
      }
      
    }
  }
  
  return(C)

}