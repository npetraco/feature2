/* 
 * File:   main.cpp
 *
 */

#include <iostream>
#include <RInside.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
/*
 * 
 */

int main(int argc, char** argv) {
    
    //RInside R(argc, argv);	   // create an embedded R instance
    //R["txt"] = "Hello, world!\n";  // pass data from C++ to R. assign a char* (string) to 'txt'
    //R.parseEvalQ("cat(txt)");      // eval the init string, ignoring any returns
    //R.parseEvalQ("txt2 <- 3");     // pass data from R to C++.
    //int x = R["txt2"];
    //cout << x << endl;
    //R.parseEvalQ("txt3 <- tempdir()");
    //std::string dir = R["txt3"];
    //cout << dir << endl;
    
    
    //cv::Mat H(10, 10, CV_8U);
    //cout << H << endl;
    
    //cv::namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    //cv::imshow( "Display window", H );                   // Show our image inside it.
    //waitKey(0);
    
    //cout << argv[2] << " "<< argv[3] << endl;
 
    uint r = strtol(argv[2], NULL, 0);
    uint c = strtol(argv[3], NULL, 0);
    
    Mat image;
    image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);   // Read the file
    
    namedWindow( "Display window", 0 );// Create a window for display.
    Size size(r, c);//the dst image size,e.g.argv[2]xargv[3]
    Mat dst;//dst image
    resize(image,dst,size);//resize image
    dst.convertTo(dst, -1, 0.5, 0);


    imshow( "Display window", dst );                   // Show our image inside it.
    //resizeWindow("Display window", 500,500);

    waitKey(0);                                          // Wait for a keystroke in the window

    return 0;
}