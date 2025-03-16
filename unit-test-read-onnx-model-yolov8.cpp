#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <iostream>

int main() {
    std::string modelPath = "yolov8m.onnx";

    // โหลดโมเดล ONNX
    cv::dnn::Net net = cv::dnn::readNetFromONNX(modelPath);
    
    if (net.empty()) {
        std::cerr << "Failed to load ONNX model!" << std::endl;
        return -1;
    }
    
    std::cout << "YOLOv8 model loaded successfully!" << std::endl;

    return 0;
}