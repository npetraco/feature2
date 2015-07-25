# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""
import sys
sys.path.append('/usr/local/lib/python2.7/site-packages')
import numpy as np
import cv2
from matplotlib import pyplot as plt
import commands

tmpdir = commands.getstatusoutput('echo $TMPDIR')[1]
tmpfile = tmpdir + 'tmpimg.jpg'

img = cv2.imread(tmpfile,0)
#type(img)
#type(img[1,1])

plt.imshow(img, cmap = 'gray', interpolation = 'bicubic')
#plt.xticks([]), plt.yticks([])  # to hide tick values on X and Y axis
#plt.show()
