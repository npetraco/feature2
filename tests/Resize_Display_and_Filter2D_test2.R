library(x3pr)
library(feature2)

#pth.fa <- "/Users/npetraco/latex/professional/DR_LABBS/Exported/Test02.x3p"
pth.fa <- "/Users/npetraco/math/Data/database/toolmark_database/Consecutive_Breach_Face_Primer_Shear_Jim/3.3/total_threshold.lms"
#pth.fa <- "/Users/npetraco/latex/papers/signal_feature/2D_tests/2A-1.x3p"
finfo.fa <- read.zeiss.lms.file(pth.fa)
#finfo.fa <- read.x3p(pth.fa)
plot3D.x3p.file(finfo.fa, 1024, 256, aspect=c(1,3,0.3), plot.type="surface")

surf <- finfo.fa[[2]]

#A little test matrix
tm <- rbind(
  c(1.2,0,6.1),
  c(NaN,4,10),
  c(255,5.8,-5.1),
  c(3,356,NA)
)
tm

#Display
dispmat <- surf
display2D.openCV(dispmat, num.rows=round(0.15*nrow(dispmat)), num.cols=round(0.15*ncol(dispmat)), bit.depth="8bit", file.ext="jpg", printQ=F)

#Resize
dim(surf)
surf.red <- Resize(surf, 256, 512)
plot3D.sip(surf.red, 512, 256, aspect=c(1,3,0.3), plot.type="surface")
#
dim(surf.red)
image(surf.red)
tmp <- finfo.fa[[1]] 
tmp$num.lines <- 256
tmp$num.pts.line <- 512
plot3D.x3p.file(list(tmp, surf.red), 512, 256, aspect=c(1,3,0.3), plot.type="surface")
plot3D.x3p.file(list(tmp, surf.red), 512, 256, aspect=c(1,3,0.3), plot.type="points")

#Filter
ker<-rbind(
  c(0,-1,0),
  c(-1,5,-1),
  c(0,-1,0)
)
ker

surf.red <- Resize(surf, 256, 512)
surf.filt <- Filter2D(surf.red, ker, "8bit")
dim(surf.filt)

#If surface was reduced:
tmp$num.lines <- 256
tmp$num.pts.line <- 512
plot3D.x3p.file(list(tmp, surf.filt), 512, 256, aspect=c(1,3,0.3), plot.type="surface")
#plot3D.x3p.file(list(tmp, surf.filt), 512, 256, aspect=c(1,3,0.3), plot.type="points")

#If surface was not reduced:
#plot3D.x3p.file(list(finfo.fa[[1]], surf.filt), 512, 256, aspect=c(1,3,0.3), plot.type="surface")


testscale(tm, "8bit",printQ=TRUE)
testscale(tm, "16bit",printQ=TRUE)
testscale(tm, "32bitfloat",printQ=TRUE)
tm

junk <- testConvert(surf,printQ=F)
