#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

/*
 * Display an image with OpenCV. Meant to be called by a system command from R 
 */

int main(int argc, char** argv) {
    
    uint r = strtol(argv[2], NULL, 0);
    uint c = strtol(argv[3], NULL, 0);
    
    Mat image;
    image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);  // Read the file
    
    namedWindow( "Display window", 0 );                // Create a window for display.
    Size size(c, r);                                   // The dst image size,e.g.argv[2]xargv[3]
    Mat dst;                                           // The dst image
    resize(image,dst,size);//resize image
    //dst.convertTo(dst, -1, 0.5, 0);

    imshow( "Display window", dst );                   // Show our image inside it.

    waitKey(0);                                        // Wait for a keystroke in the window

    return 0;
}