#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
#include <assert.h>
int main()
{
    //get figure from path
    cv::Mat src = cv::imread("../data pack/apple.png");
    assert(src.channels() == 3);

    //convert from BGR to HSV
    cv::Mat hsv;
    cv::cvtColor(src,hsv,cv::COLOR_BGR2HSV);

    //get the range of HSV color, mainly red and orange (Binarization)
    cv::Mat hsv_part1, hsv_part2;
    cv::inRange(hsv,cv::Scalar(0,43,46),cv::Scalar(19,255,255),hsv_part1);
    cv::inRange(hsv,cv::Scalar(156,43,46),cv::Scalar(180,255,255),hsv_part2);
    
    //get the union of two parts.
    cv::Mat ones_mat = cv::Mat::ones(cv::Size(src.cols,src.rows),CV_8UC1);
    cv::Mat hsv_result = 255*(ones_mat-(ones_mat-hsv_part1/255).mul(ones_mat-hsv_part2/255));
    
    //wave filting (median)
    cv::Mat hsv_median;
    cv::medianBlur(hsv_result,hsv_median,5);

    //morphology, first open, then close
    cv::Mat element;
    element = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(21,21));
    cv::Mat result;
    cv::morphologyEx(hsv_median,result,cv::MORPH_OPEN,element);
    cv::morphologyEx(result,result,cv::MORPH_CLOSE,element);

    //get contour
    std::vector< std::vector<cv::Point> > contour;
    std::vector< cv::Vec4i> hierachy;
    cv::findContours(result,contour,hierachy, cv::RETR_TREE,cv::CHAIN_APPROX_NONE);

    //draw the minimum enclosing rectangle. From: https://blog.csdn.net/dcrmg/article/details/52260699
    for(int i=0;i<contour.size();i++)
	{		
		cv::RotatedRect rect=cv::minAreaRect(contour[i]);
		cv::Point2f P[4];
		rect.points(P);
		for(int j=0;j<=3;j++)
		{
			line(src,P[j],P[(j+1)%4],cv::Scalar(255),2);
		}
	}

    //show the picture
    cv::imshow("src",src);
    cv::imwrite("../find_apple.jpg",src);
    cv::waitKey(0);
    return 0;
}