library(x3pr)
library(feature2)

pth.fa <- "/Users/npetraco/latex/professional/DR_LABBS/Exported/Test02.x3p"
#pth.fa <- "/Users/npetraco/math/Data/database/toolmark_database/Consecutive_Breach_Face_Primer_Shear_Jim/3.3/total_threshold.lms"
#pth.fa <- "/Users/npetraco/latex/papers/signal_feature/2D_tests/2A-1.x3p"
#finfo.fa <- read.zeiss.lms.file(pth.fa)
finfo.fa <- read.x3p(pth.fa)
plot3D.x3p.file(finfo.fa, 1024, 256, aspect=c(1,3,0.3), plot.type="surface")

surf <- finfo.fa[[2]]
surf2 <- testConvert(surf, max(surf, na.rm = T), min(surf, na.rm = T), "32bitfloat")
open3d()
plot3D.x3p.file(list(finfo.fa[[1]],surf2), 256, 256, aspect=c(1,1,0.3), plot.type="surface")


#A little test matrix
tm <- rbind(
  c(1.2,0,6.1),
  c(NaN,4,10),
  c(255,5.8,-5.1)
)
tm
max(tm)
max(tm, na.rm = T)
WriteMat(tm, max(tm, na.rm = T), min(tm, na.rm = T), "8bit")
testConvert(tm, max(tm, na.rm = T), min(tm, na.rm = T), "8bit")
tm

dispmat <- surf
display2D.openCV(dispmat, num.rows=round(0.15*nrow(dispmat)), num.cols=round(0.15*ncol(dispmat)), bit.depth="8bit", file.ext="jpg", printQ=F)
display2D.openCV(dispmat, num.rows=round(1*nrow(dispmat)), num.cols=round(1*ncol(dispmat)), bit.depth="8bit", file.ext="jpg", printQ=F)
