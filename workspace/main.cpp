#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <iostream>

using namespace cv;
using namespace dnn;
using namespace std;

void drawBoundingBox(Mat &img, const vector<float> &box, float confidence)
{
    // สมมติว่าโมเดลให้ค่า (center_x, center_y, width, height)
    float center_x = box[0] * img.cols;
    float center_y = box[1] * img.rows;
    float w = box[2] * img.cols;
    float h = box[3] * img.rows;

    // คำนวณ x_min และ y_min
    int x_min = static_cast<int>(center_x - w / 2);
    int y_min = static_cast<int>(center_y - h / 2);
    int x_max = static_cast<int>(center_x + w / 2);
    int y_max = static_cast<int>(center_y + h / 2);

    // วาดกรอบสี่เหลี่ยม
    rectangle(img, Point(x_min, y_min), Point(x_max, y_max), Scalar(0, 255, 0), 2);

    // แสดงค่า Confidence
    string label = format("Conf: %.2f", confidence);
    putText(img, label, Point(x_min, y_min - 10), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 2);
}

int main()
{
    // โหลดโมเดล ONNX
    Net net = readNetFromONNX("yolov8s.onnx");

    // โหลดภาพ
    Mat img = imread("image.png");
    if (img.empty())
    {
        cerr << "Error: Image not found!" << endl;
        return -1;
    }

    // แปลงภาพเป็น Blob
    Mat blob;
    blobFromImage(img, blob, 1.0 / 255.0, Size(640, 640), Scalar(), true, false);
    // Size inputSize = Size(img.cols, img.rows); // ใช้ขนาดภาพจริง
    // blobFromImage(img, blob, 1.0 / 255.0, inputSize, Scalar(), true, false);

    // ใส่ blob เข้าโมเดล
    net.setInput(blob);

    // ทำ Inference
    Mat output = net.forward();

    // ดึงข้อมูลจาก Output (1 x 84 x 8400)
    const float *data = reinterpret_cast<float *>(output.data);
    int numBoxes = output.size[2];

    // วนลูปดึง Bounding Box
    for (int i = 0; i < numBoxes; i++)
    {
        float confidence = data[4]; // ค่าความมั่นใจ
        if (confidence > 0.5)
        {                                      // กรองเฉพาะค่าที่มีความมั่นใจสูง
            vector<float> box(data, data + 4); // (x, y, w, h)
            // vector<float> box = {data[0] / 640.0, data[1] / 640.0, data[2] / 640.0, data[3] / 640.0};
            drawBoundingBox(img, box, confidence);
        }
        data += 84; // ไปยัง Bounding Box ถัดไป
    }

    // แสดงภาพที่มี Bounding Box
    imshow("Detected Objects", img);
    waitKey(0);

    return 0;
}
