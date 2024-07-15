#pragma once
#include <opencv2/opencv.hpp>
#include <string>

void display(std::string filename) {
    // 载入PPM图像
    cv::Mat image = cv::imread(filename);
    if (image.empty()) {
        std::cerr << "图像加载失败！请检查路径是否正确。" << std::endl;
        return;
    }
    cv::imshow("Output", image);
    cv::imwrite("output.jpg", image);
    cv::waitKey(0);
}