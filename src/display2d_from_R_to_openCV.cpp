#include <Rcpp.h>
#include <cstdlib>
#include <opencv2/opencv.hpp>
#include "internals.hpp"

using namespace Rcpp;
using namespace cv;
using namespace std;

// [[Rcpp::export]]
void Display2D_openCV(NumericMatrix dmat, int rowWidth, int colWidth, std::string bitdepth, std::string file_ext, bool printQ, std::string exe_path) {
      
  //Copy?? input matrix dmat to an openCV container and scale/cast to required bit depth representation.
  Mat H2;
  NumericMatrix2openCVMat(dmat, H2);
  scale64ToDepth(H2, bitdepth);
  
  if(printQ==true) {
    Rcout << H2 << endl;
  }

  //Trying to launch an openCV window from R or RStudio causes a segfault. Our work-around
  //is to call a stand-alone openCV program in inst/display2d to open up an openCV window and render the image.
  //First however we need an image to render. Get the temp directory path and write the image there.
  std::string tmpdir = getenv("TMPDIR");
  std::string tmpfile = tmpdir + "tmpimg." + file_ext;  
  //Rcout << "Image written to temporary file: " << tmpfile << endl;
  
  //cv::imwrite to temp directory
  //Delete the file(s) it (they're) already in there.
  std::string rmf = "rm -f " + tmpdir + "tmpimg.*";
  std::system(rmf.c_str());
  imwrite( tmpfile, H2 );

  //system call to inst/display2d, tmp path command line arg.
  std::string plot_cmd = exe_path + " " + tmpfile + " " + to_string(rowWidth) + " " + to_string(colWidth);
  //Rcout << plot_cmd << endl;
  system(plot_cmd.c_str());
  
}