library(x3pr)

#pth <- "/Users/npetraco/math/Data/database/toolmark_database/screwdriver/full_screwdrivers_x3p/2A/2A-1.x3p"
pth.tm <- "/Users/npetraco/latex/papers/signal_feature/2D_tests/2A-1.x3p"
pth.fa <- "/Users/npetraco/latex/professional/DR_LABBS/Exported/Test02.x3p"
finfo.tm <- read.x3p(pth.tm)
finfo.fa <- read.x3p(pth.fa)

print.x3p.file.info(finfo.tm[[1]])
print.x3p.file.info(finfo.fa[[1]])

plot3D.x3p.file(finfo.tm, 1024, 80, aspect=c(1,3,0.3), plot.type="surface")
plot3D.x3p.file(finfo.fa, 256, 256, aspect=c(1,1,0.3), plot.type="surface")

#Small test matrix:
tm <- rbind(
  c(1.2,3.5,6.1),
  c(NaN,4,10),
  c(258,5.8,-5.1)
)
tm

#A Zeiss surface in the 16-bit range:
pth.zf <- system.file("extdata", "glock.lms", package="x3pr")
finfo.zf <- read.zeiss.lms.file(pth.zf)
finfo.zf16 <- finfo.zf
finfo.zf16[[2]] <-  (1/finfo.zf[[1]]$height.inc) * finfo.zf[[2]] 
plot3D.x3p.file(finfo.zf16, 1024, 80, aspect=c(1,3,0.3), plot.type="surface")

dim(finfo.zf16[[2]])
chunk16 <- finfo.zf16[[2]][1:100,1:500]
min(chunk16)
max(chunk16)
image(chunk16)
display2D.openCV(chunk16, num.rows=100, num.cols=500, bit.depth="16bit", file.ext="jpg", printQ=F)

chunk.rescaled <- rescale.z(chunk16, z.bitdepth = 8, zero.maskQ = F)
display2D.openCV(chunk.rescaled, num.rows=500, num.cols=500, bit.depth="8bit", file.ext="jpg", printQ=F)
display2D.openCV(chunk.rescaled, num.rows=100, num.cols=100, bit.depth="8bit", file.ext="jpg", printQ=F)
display2D.openCV(chunk.rescaled, num.rows=nrow(chunk.rescaled), num.cols=ncol(chunk.rescaled), bit.depth="8bit", file.ext="jpg", printQ=F)
display2D.openCV(chunk.rescaled, num.rows=round(1*nrow(chunk.rescaled)), num.cols=round(1*ncol(chunk.rescaled)), bit.depth="8bit", file.ext="jpg", printQ=F)


finfo.fa.rescaled <- rescale.z(finfo.fa[[2]], z.bitdepth = 8, zero.maskQ = F)
display2D.openCV(finfo.fa.rescaled, num.rows=500, num.cols=500, bit.depth="8bit", file.ext="jpg", printQ=F)
display2D.openCV(finfo.fa.rescaled, num.rows=500, num.cols=500, bit.depth="8bit", file.ext="png", printQ=F)
display2D.openCV(finfo.fa.rescaled, num.rows=nrow(finfo.fa.rescaled), num.cols=ncol(finfo.fa.rescaled), bit.depth="8bit", file.ext="jpg", printQ=F)
display2D.openCV(finfo.fa.rescaled, num.rows=round(1*nrow(finfo.fa.rescaled)), num.cols=round(1*ncol(finfo.fa.rescaled)), bit.depth="8bit", file.ext="jpg", printQ=F)
