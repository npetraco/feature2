# feature2 

A small R package to pull in handy routenes from openCV (and eventually vlfeat and scikit-image) for processing of 3D surfaces, typically obtained from a 3D microscope. Though this library is independent of the x3pr R package for basic x3p data format handeling, we are designing it with the x3p format in mind. 

* Under heavy development. Current capabilities:
  * Interactive 3D surface and point plotting
  * Hole detection
  * Hole filling
  * Inner and outer rectangular region of interest (ROI) cropping
  * Inner and outer circular region of interest (ROI) cropping
  * Surface data bitdepth conversion
  * Surface resizing
  * 2D filtering (multiple types: blur, gaussian, sobel, wavelet)
  * 2D FFT magnitudes with plots
  * 2D CCF
  * Python interface via boost.python

* The package requies that openCV, python, boost, vlfeat (the C library. The MATLAB parts are not necessary) is installed. It is currently being developed with openCV 2.4.11, but we will change over to 3.0 soon.

* We have only tested this on OSX and some compiler flags are unique to our programming enviornment. Take a look at /src/Makvars if you want to begin to figure out how to compile for your enviornment (though we don't recommend that yet, and probably won't for a while. The package is under heavy development.)  It should compile in Linux with clang installed and some fiddeling with the compiler flags in Makevars. Compiling with gcc should only take a little extra coaxing. We intend that this package will work seemlessly on Windows, OSX and Linux, but that is still some time down the road.
