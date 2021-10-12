#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <assert.h>
int main()
{
    cv::VideoCapture capture("../video.mp4");
    cv::Mat src;
    while (capture.read(src)) {
//        cv::imshow("src",src);
        cv::Mat hsv;
        cv::cvtColor(src,hsv,cv::COLOR_BGR2HSV);

        cv::Mat hsv_part1, hsv_part2;
        cv::inRange(hsv,cv::Scalar(0,43,46),cv::Scalar(25,255,255),hsv_part1);
        cv::inRange(hsv,cv::Scalar(156,43,46),cv::Scalar(180,255,255),hsv_part2);

        cv::Mat ones_mat = cv::Mat::ones(cv::Size(src.cols,src.rows),CV_8UC1);
        cv::Mat hsv_result = 255*(ones_mat-(ones_mat-hsv_part1/255).mul(ones_mat-hsv_part2/255));

        cv::Mat hsv_median;
        cv::medianBlur(hsv_result,hsv_median,5);

        cv::Mat element;
        element = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(7,7));
        cv::Mat result;
        cv::morphologyEx(hsv_median,result,cv::MORPH_CLOSE,element);

        cv::imshow("result",result);
        std::vector< std::vector<cv::Point> > contour;
        std::vector< cv::Vec4i> hierachy;

        cv::findContours(result,contour,hierachy, cv::RETR_TREE,cv::CHAIN_APPROX_NONE);
    
        for (int i=0;i<contour.size();++i) {
            if (hierachy[i][3] == -1 && hierachy[hierachy[i][2]][0] == -1) {
                cv::RotatedRect rect=cv::minAreaRect(contour[i]);
		        cv::Point2f P[4];
		        rect.points(P);
		        for(int j=0;j<=3;j++)
		        {
			        line(src,P[j],P[(j+1)%4],cv::Scalar(0,255,0),2);
		        }
            }
            else if (hierachy[i][2] == -1 && hierachy[i][3] == -1) {
                cv::RotatedRect rect=cv::minAreaRect(contour[i]);
		        cv::Point2f P[4];
		        rect.points(P);
		        for(int j=0;j<=3;j++)
		        {
			        line(src,P[j],P[(j+1)%4],cv::Scalar(255,0,0),2);
		        }
            }
            else if (hierachy[i][3] == -1 && hierachy[hierachy[i][2]][0] != -1) {
                cv::RotatedRect rect=cv::minAreaRect(contour[i]);
		        cv::Point2f P[4];
		        rect.points(P);
		        for(int j=0;j<=3;j++)
		        {
			        line(src,P[j],P[(j+1)%4],cv::Scalar(255,0,0),2);
		        }
            }
        }
    
        cv::imshow("src",src);
        cv::waitKey(30);
    }
    return 0;
}