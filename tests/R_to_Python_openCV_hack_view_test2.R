library(x3pr)
library(feature2)

#Small test matrix:
tm <- rbind(
  c(1.2,3.5,6.1),
  c(0,4,10),
  c(255,5.8,-5.1)
)
tm  
tm.rescaled <- rescale.z(tm, z.bitdepth = 8, zero.maskQ = F)
tm.rescaled

Write_To_Tmpfile_From_R_Via_openCV(tm.rescaled, bitdepth="8bit", file_ext="jpg", printQ=T)
#display2D.openCV(finfo.fa.rescaled, num.rows=round(1*nrow(finfo.fa.rescaled)), num.cols=round(1*ncol(finfo.fa.rescaled)), bit.depth="8bit", file.ext="jpg", printQ=F)


initialize_python()
pyrun('execfile("/Users/npetraco/codes/R/feature2/tests/load_in.py")')
finalize_python()

system("python import sys")
system("python /Users/npetraco/codes/R/feature2/tests/load_in.py")

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