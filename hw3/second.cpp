#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
int main()
{
    std::vector< Point2f > imagePoint;
    std::vector< std::vector< Point2f> > imagePoint_vec;
    std::vector < Point3f > worldPoint;
    std::vector< std::vector< Point3f> > worldPoint_vec;
    Mat cameraMatrix = (Mat_<double>(3,3) << 9.1234465679824348e+02, 0., 6.4157634413436961e+02, 0.,
       7.6573154962089018e+02, 3.6477945186797331e+02, 0., 0., 1. );
    Mat distCoeffs = (Mat_<double>(5,1) << 0., -4.2669718747763807e-01, 2.6509688616309912e-01,
       -5.3624979910268683e-04, -4.1011485564833132e-04);
    Mat rvecs;
    Mat tvecs;

    const int board_w = 9, board_h = 6;
    const int board_n = board_w * board_h;
    Size board_size( 9, 6 );
    Size cell_size(10,10);

    Mat gray_img, drawn_img;
    int found, successes = 0;
    Size img_size;

    int cnt = 0;
    int k = 0, n = 0;

    //Thanks to fellows for this part.
     for (int j = 0; j < board_h; j++ ) {
        for (int k = 0; k < board_w; k++ ){
            Point3f pt;
            pt.x = k * cell_size.width;
            pt.y = j * cell_size.height;
            pt.z = 0;
            worldPoint.push_back( pt );
        }
    }

    for (int i = 0; i < 41; i++){
        cv::Mat src0 = cv::imread(std::string("../data pack/chess/")+std::__cxx11::to_string(i).append(".jpg"));
        if ( !cnt ) {
            img_size.width = src0.cols;
            img_size.height = src0.rows;
        }
        found = findChessboardCorners( src0, board_size, imagePoint );
        if ( found && imagePoint.size() == board_n ) {
            successes++;
            cvtColor( src0, gray_img, COLOR_BGR2GRAY );
            //threshold(gray_img,gray_img,55,255,THRESH_OTSU);
            find4QuadCornerSubpix( gray_img, imagePoint, Size( 5, 5 ) );

            solvePnP(worldPoint,imagePoint,cameraMatrix,distCoeffs,rvecs,tvecs);
            std::cout<<"image:"<<i<<std::endl;
            std::cout<<"rvec: "<<rvecs<<std::endl;
            std::cout<<"tvec: "<<tvecs<<std::endl;

            imagePoint_vec.push_back( imagePoint );
            drawn_img = src0.clone();
            drawChessboardCorners( drawn_img, board_size, imagePoint, found );
            imshow( "corners", drawn_img );
            waitKey( 50 );
        } else
            std::cout << "\tbut failed to found all chess board corners in this image" << std::endl;
        imagePoint.clear();
        cnt++;
    }
    return 0;
}