library(x3pr)
library(feature2)

pth.fa <- "/Users/npetraco/latex/professional/DR_LABBS/Exported/Test02.x3p"
#pth.fa <- "/Users/npetraco/math/Data/database/toolmark_database/Consecutive_Breach_Face_Primer_Shear_Jim/3.3/total_threshold.lms"
#pth.fa <- "/Users/npetraco/latex/papers/signal_feature/2D_tests/2A-1.x3p"
#finfo.fa <- read.zeiss.lms.file(pth.fa)
finfo.fa <- read.x3p(pth.fa)
plot3D.x3p.file(finfo.fa, 256, 256, aspect=c(1,1,0.3), plot.type="surface")

surf <- finfo.fa[[2]]

ker<-rbind(
  c(0,-1,0),
  c(-1,5,-1),
  c(0,-1,0)
)
ker

surf.filt <- Filter2D(surf,ker,"8bit")
dim(surf.filt)
plot3D.x3p.file(list(finfo.fa[[1]], surf.filt), 256, 256, aspect=c(1,1,0.3), plot.type="surface")

dispmat <- surf.filt
display2D.openCV(dispmat, num.rows=round(0.15*nrow(dispmat)), num.cols=round(0.15*ncol(dispmat)), bit.depth="8bit", file.ext="jpg", printQ=F)

surf.filt.red <- Resize(surf.filt, 100, 100)
image(surf.filt.red)
tmp <- finfo.fa[[1]]
tmp$num.lines <- 100
tmp$num.pts.line <- 100
plot3D.x3p.file(list(tmp, surf.filt.red), 100, 100, aspect=c(1,1,0.3), plot.type="surface")
plot3D.x3p.file(list(tmp, surf.filt.red), 100, 100, aspect=c(1,1,0.3), plot.type="points")

