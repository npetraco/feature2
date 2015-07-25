//#include "internals.hpp"
#include <Rcpp.h>
#include <cstdlib>
#include <opencv2/opencv.hpp>
#include "internals.hpp"

using namespace Rcpp;
using namespace cv;
using namespace std;

// [[Rcpp::export]]
void Write_To_Tmpfile_From_R_Via_openCV(NumericMatrix dmat, std::string bitdepth, std::string file_ext, bool printQ) {
  
  //Copy input matrix dmat to an openCV container and cast to required bit depth representation.
  //CAUTION: ASSUMES data in dmat has been scaled to required bit depth! See R function: rescale.z.R
  Mat H2 = NumericMatrix2ImageType(dmat, bitdepth);
  
  if(printQ==true) {
    Rcout << H2 << endl;
  }

  //Trying to launch an openCV window from R or RStudio causes a segfault. Our workaround
  //is to call a stand-alone openCV program in inst/display2d to open up an openCV window and render the image.
  //First however we need an image to render. Get the temp directory path and write the image there.
  std::string tmpdir = getenv("TMPDIR");
  std::string tmpfile = tmpdir + "tmpimg." + file_ext;  
  
  //cv::imwrite to temp directory
  //Delete the file(s) it (they're) already in there.
  std::string rmf = "rm -f " + tmpdir + "tmpimg.*";
  std::system(rmf.c_str());
  imwrite( tmpfile, H2 );
  Rcout << "Image written to temporary file: " << tmpfile << endl;
  
}