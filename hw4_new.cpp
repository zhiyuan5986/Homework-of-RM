#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <Eigen/Dense>
#include <opencv2/core/eigen.hpp>
#include <iostream>
#include <cstdio>
#include <cstring>
int main()
{
    const int COLS = 2208, ROWS = 1242;
    cv::Mat depth = cv::imread("../data pack/1_dpt.tiff",cv::IMREAD_ANYDEPTH);
    cv::Mat img = cv::imread("../data pack/1.jpg",0);
    cv::imshow("initial",img);
    cv::FileStorage reader("../data pack/data.yml",cv::FileStorage::READ);
    cv::Mat c_mat, p_mat;
    Eigen::Matrix<double, 3, 3> C;
    Eigen::Matrix<double, 4, 4> P;
    reader["C"] >> c_mat; // camera matrix
    reader["D"] >> p_mat; // 
    cv::cv2eigen(p_mat,P);
    
    double fx = c_mat.at<double>(0,0);
    double fy = c_mat.at<double>(1,1);
    double cx = c_mat.at<double>(0,2);
    double cy = c_mat.at<double>(1,2);

    std::cout << fx << ' ' << fy << ' ' << cx << ' ' << cy << std::endl;
    std::cout << c_mat << std::endl;
    std::cout << p_mat << std::endl;

    std::cout << depth.rows << ' ' << depth.cols << std::endl;
    std::cout << img.rows << ' ' << img.cols << std::endl;

    // Eigen::Vector2d p(0,0);
    cv::Mat img_ = cv::Mat(ROWS,COLS,CV_8UC1);
    for (int v=0;v<ROWS;++v) {
        for (int u=0;u<COLS;++u) {
            double x = (u-cx)/fx,y=(v-cy)/fy;
            float d = depth.ptr<float>(v)[u]; // 深度值
            x *= d;
            y *= d;
            Eigen::Vector4d q(x,y,d,1);
            

            Eigen::Vector4d q_ = P*q;
            double x_ = q_(0);
            double y_ = q_(1);
            double depth_ = q_(2);
            double u_ = fx*(x_/depth_)+cx;
            double v_ = fy*(y_/depth_)+cy;

            if (u_ >= 0 && v_ >= 0 && u_ < COLS && v_ < ROWS) {
                img_.at<uchar>((int)v_,(int)u_) = img.at<uchar>(v,u);
                // std::cout << v_ << ' ' << u_ << std::endl;
            }
            // else {
            //     img_.at<uchar>((int)v_,(int)u_) = 0;
            // }
            // p[5] = color.data[v * color.step + u * color.channels()];   // blue
            // p[4] = color.data[v * color.step + u * color.channels() + 1]; // green
            // p[3] = color.data[v * color.step + u * color.channels() + 2]; // red
            //q = Vector3d(u,v,depth.ptr<float>(v)[u]);
            // q(0) = i;
            // q(1) = j;
            // q(2) = depth.at<uint16_t>(i,j);
            // cv::eigen2cv(q,q_);
            // p_ = c_mat*p_mat*q_;
            // cv::eigen2cv(p,p_);
            // img_.at<cv::Point3f>(static_cast<int>(p_[0]),static_cast<int>(p_[1])) = img.at<cv::Point3f>(i,j);
        }
    }
    cv::imshow("result",img_);
    cv::waitKey(0);
    return 0;
}