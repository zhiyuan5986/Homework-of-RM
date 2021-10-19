#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>
#include <vector>

using namespace cv;

void doPerspectiveTransform( Mat input, Mat& output ) {
    std::vector<Point2f> srcQuad( 4 ), dstQuad( 4 );
    output = input.clone();
    srcQuad[0].x = 603, srcQuad[1].x =  773 , srcQuad[2].x = 766, srcQuad[3].x = 596;
    srcQuad[0].y = 390, srcQuad[1].y = 392, srcQuad[2].y = 333, srcQuad[3].y = 331;
    dstQuad[0].x = 0, dstQuad[1].x = 170, dstQuad[2].x = 170, dstQuad[3].x = 0;
    dstQuad[0].y = 60, dstQuad[1].y = 60, dstQuad[2].y = 0, dstQuad[3].y = 0;

    Mat warp_matrix = getPerspectiveTransform( srcQuad, dstQuad );

    warpPerspective( input, output, warp_matrix, Size( 170, 60 ) );
}


int main() {
    Mat img = imread("../data pack/car.jpg");
    Mat result;

    doPerspectiveTransform( img, result );

    namedWindow( "input" );
    namedWindow( "output" );

    imshow( "input", img );
    imshow( "output", result );

    waitKey( 0 );

    destroyWindow( "input" );
    destroyWindow( "output" );
    return 0;
}