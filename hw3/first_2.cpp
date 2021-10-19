#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>
#include <stdio.h>
#include <vector>

#define CV_EVENT_LBUTTONDOWN 1
using namespace cv;
Mat src = imread("../data pack/car.jpg");
Mat dst = src.clone();
Mat result;
// static int X=0;
// static int Y=0;
// Point pt = Point(0,0);
void on_mouse(int event, int x, int y, int flags, void* ustc)
{
    char temp[16];
    // font font;
    // initFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 1, CV_AA);

    if (event == CV_EVENT_LBUTTONDOWN) {
        //cvCopy(dst,src);   
        std::vector<Point2f> *srcQuad =  (std::vector<Point2f>*)ustc;      
        sprintf(temp, "(%d,%d)", x, y);
        srcQuad->push_back(Point2f(x,y));
		putText(dst, temp, Point(x,y), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0, 255));
        //putText(src, temp, pt, &font, Scalar(255, 255, 255, 0));
        //circle(src, pt, 2, Scalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);
        // dst = src.clone();
        imshow("input", dst);
    }
}

void doPerspectiveTransform( Mat input, Mat& output ) {
    std::vector<Point2f> srcQuad, dstQuad( 4 );
    output = input.clone();
    namedWindow( "input" );
    imshow( "input", input );
    setMouseCallback("input",on_mouse,&srcQuad);
    waitKey(0);
    // srcQuad[i].x = pt.x;
    // srcQuad[i].y = pt.y;
    // std::cout << i << ' ' << pt.x << ' ' << pt.y << std::endl;
    // srcQuad[0].x = 603, srcQuad[1].x =  773 , srcQuad[2].x = 766, srcQuad[3].x = 596;
    // srcQuad[0].y = 390, srcQuad[1].y = 392, srcQuad[2].y = 333, srcQuad[3].y = 331;
    for (int i=0;i<4;++i) std::cout << srcQuad[i].x << ' ' << srcQuad[i].y << std::endl;
    dstQuad[0].x = 0, dstQuad[1].x = 170, dstQuad[2].x = 170, dstQuad[3].x = 0;
    dstQuad[0].y = 60, dstQuad[1].y = 60, dstQuad[2].y = 0, dstQuad[3].y = 0;

    Mat warp_matrix = getPerspectiveTransform( srcQuad, dstQuad );

    warpPerspective( input, output, warp_matrix, Size( 170, 60 ) );
}


int main() {


    doPerspectiveTransform( src, result );

    namedWindow( "output" );
    imshow( "output", result );

    waitKey( 0 );

    destroyWindow( "input" );
    destroyWindow( "output" );
    return 0;
}