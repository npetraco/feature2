library(x3pr)
library(feature2)

pth.fa <- "/Users/npetraco/latex/professional/DR_LABBS/Exported/Test02.x3p"
finfo.fa <- read.x3p(pth.fa)

surf <- finfo.fa[[2]]
display2D.openCV(surf, num.rows=round(.1*nrow(surf)), num.cols=round(.1*ncol(surf)), bit.depth="8bit", file.ext="jpg", printQ=F)

initialize_python()
pyrun("import os")
pyrun("import /Users/npetraco/codes/R/feature2/tests/load_in.py")
pyrun("os.system('/Users/npetraco/codes/R/feature2/tests/load_in.py')")
pyrun("execfile('/Users/npetraco/codes/R/feature2/tests/load_in.py')")

pyrun("import subprocess")
pyrun("subprocess.Popen('/Users/npetraco/codes/R/feature2/tests/load_in.py 1', shell=True)")
x11()
