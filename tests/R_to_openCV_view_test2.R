library(x3pr)

#A primershear in the 16-bit range:
pth.zf <- "/Users/npetraco/math/Data/database/toolmark_database/Consecutive_Breach_Face_Primer_Shear_Jim/3.3/total_threshold.lms"
finfo.zf <- read.zeiss.lms.file(pth.zf)
dim(finfo.zf[[2]])
finfo.zf16 <- finfo.zf
finfo.zf16[[2]] <-  (1/finfo.zf[[1]]$height.inc) * finfo.zf[[2]] 
plot3D.x3p.file(finfo.zf16, 1024, 80, aspect=c(1,3,0.3), plot.type="surface")


finfo.zf16.rescaled <- rescale.z(finfo.zf16[[2]], z.bitdepth=list("custom.dynamic.range",20, 256/2), zero.maskQ = F)
min(finfo.zf16.rescaled)
max(finfo.zf16.rescaled)
display2D.openCV(finfo.zf16.rescaled, num.rows=500, num.cols=500, bit.depth="8bit", file.ext="jpg", printQ=F)
display2D.openCV(finfo.zf16.rescaled, num.rows=round(1*nrow(finfo.zf16.rescaled)), num.cols=round(1*ncol(finfo.zf16.rescaled)), bit.depth="8bit", file.ext="jpg", printQ=F)
image(finfo.zf16.rescaled)
