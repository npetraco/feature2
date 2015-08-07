#include <iostream>
#include <RInside.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

/*
 * 
 */

int main(int argc, char** argv) {
    
    //RInside R(argc, argv);		    // create an embedded R instance
    //R["txt"] = "Hello, world!\n";	// assign a char* (string) to 'txt'
    //R.parseEvalQ("cat(txt)");       // eval the init string, ignoring any returns
    
    unsigned int r = strtol(argv[2], NULL, 0);
    unsigned int c = strtol(argv[3], NULL, 0);
    
    Mat src;
    src = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);  // Read the file
    
    /// Create Window
    char* source_window = "Source";
    namedWindow( source_window, CV_WINDOW_AUTOSIZE );
    Size size(c, r);                                   // The dst image size,e.g.argv[2]xargv[3]
    Mat dst;                                           // The dst image
    resize(src,dst,size);//resize image
    imshow( source_window, dst );

    
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    
    /// Find contours
    findContours( src, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    
    /// Draw contours
    RNG rng(12345);
    Mat drawing = Mat::zeros( src.size(), CV_8U);
    for( int i = 0; i< contours.size(); i++ ) {
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
     }

    /// Show in a window
    Mat dst_drawing;
    resize(drawing,dst_drawing,size);//resize image

    namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
    imshow( "Contours", dst_drawing );
    
    waitKey(0);
    return 0;
}