#include "internals.hpp"

using namespace Rcpp;
using namespace cv;
using namespace std;

//Internal. Not exported to R. Intended for conversion of doubles matrix input from R into openCV 
//featue extraction routines. Assumes data is already scaled to required dynamic range for the type

void tawocvm(NumericMatrix dmat) {
    
}