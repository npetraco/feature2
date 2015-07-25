library(x3pr)
library(feature2)

#A primershear in the 16-bit range:
pth.zf <- "/Users/npetraco/math/Data/database/toolmark_database/Consecutive_Breach_Face_Primer_Shear_Jim/3.3/total_threshold.lms"
finfo.zf <- read.zeiss.lms.file(pth.zf)
dim(finfo.zf[[2]])
finfo.zf16 <- finfo.zf
finfo.zf16[[2]] <-  (1/finfo.zf[[1]]$height.inc) * finfo.zf[[2]] 
plot3D.x3p.file(finfo.zf16, 1024, 80, aspect=c(1,3,0.3), plot.type="surface")

finfo.zf16.rescaled <- rescale.z(finfo.zf[[2]], z.bitdepth = 8, zero.maskQ = F)
plot3D.x3p.file(list(finfo.zf16[[1]], finfo.zf16.rescaled), 1024, 80, aspect=c(1,3,0.3), plot.type="surface")

Write_To_Tmpfile_From_R_Via_openCV(finfo.zf[[2]], bitdepth="64bit", file_ext="jpg", printQ=F)
#display2D.openCV(finfo.fa.rescaled, num.rows=round(1*nrow(finfo.fa.rescaled)), num.cols=round(1*ncol(finfo.fa.rescaled)), bit.depth="8bit", file.ext="jpg", printQ=F)

junk <- finfo.fa[[2]] - min(finfo.fa[[2]], na.rm = T)
fl <- mean(junk, na.rm=T)
junk[which(is.nan(junk) ==T, arr.ind = T)] <- fl
junk <- 255.0/( max(junk, na.rm = T) - min(junk, na.rm = T) ) * junk

max(junk, na.rm = T)
min(junk, na.rm = T)
Write_To_Tmpfile_From_R_Via_openCV(junk, bitdepth="64bit", file_ext="jpg", printQ=F)

plot3D.x3p.file(finfo.zf, 1024, 80, aspect=c(1,3,0.3), plot.type="surface")
plot3D.x3p.file(list(finfo.zf[[1]], junk), 1024, 80, aspect=c(1,3,0.3), plot.type="surface")

initialize_python()
pyrun('execfile("/Users/npetraco/codes/R/feature2/tests/load_in.py")')
finalize_python()

#Copy variables to Python
numvec_to_python("x", x) 
numvec_to_python("sx", sx)
numvec_to_python("cx", cx)

#Below wont work, bit we can mod executed script.
#Set plot size
pyrun("plt.rcParams.update({'figure.figsize' : (7,4)})") 
#Create plots
pyrun("plt.plot(x, sx)")
pyrun("plt.plot(x, cx, '--r', linewidth=2) ")
pyrun("plt.legend(('sin(x)', 'cos(x)'))")
pyrun("plt.savefig('../figure/2015-04-02-pyplot.png')")
#pyrun("plt.show()") #Uncomment this line to show the plot