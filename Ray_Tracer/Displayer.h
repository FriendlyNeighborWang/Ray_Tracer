#pragma once
#include <opencv2/opencv.hpp>
#include <string>

void display(std::string filename) {
    // ����PPMͼ��
    cv::Mat image = cv::imread(filename);
    if (image.empty()) {
        std::cerr << "ͼ�����ʧ�ܣ�����·���Ƿ���ȷ��" << std::endl;
        return;
    }
    cv::imshow("Output", image);
    cv::imwrite("output.jpg", image);
    cv::waitKey(0);
}