#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>

// Y�z�n merkezini hesaplayan fonksiyon
cv::Point calculateFaceCenter(const cv::Rect& face) {
    return cv::Point(face.x + face.width / 2, face.y + face.height / 2);
}

// �er�evenin merkezini d�nd�ren fonksiyon
cv::Point getFrameCenter(const cv::Mat& frame) {
    return cv::Point(frame.cols / 2, frame.rows / 2);
}

// �ki nokta aras�ndaki mesafeyi hesaplayan fonksiyon
double calculateDistance(const cv::Point& p1, const cv::Point& p2) {
    return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
}

// Y�z�n konumunu kontrol eden ve uyar� veren fonksiyon
void checkFacePosition(cv::Mat& frame, const cv::Rect& face, const cv::Point& frameCenter) {
    cv::Point faceCenter = calculateFaceCenter(face);
    double distance = calculateDistance(faceCenter, frameCenter);

    double threshold = 100.0; // Mesafe e�i�i

    if (distance > threshold) {
        std::string warningText = "Y�z merkezde de�il!";
        cv::putText(frame, warningText, cv::Point(50, 50), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2);
        cv::rectangle(frame, face, cv::Scalar(0, 0, 255), 2); // Y�z� k�rm�z� kutu ile g�ster
    }
    else {
        cv::rectangle(frame, face, cv::Scalar(0, 255, 0), 2); // Y�z� ye�il kutu ile g�ster
    }
}

int main() {
    // Y�z tespiti i�in Haar Cascade y�kleme
    cv::CascadeClassifier face_cascade;
    face_cascade.load("C:/Users/Okutgen/Downloads/haarcascade_frontalface_default.xml");

    // Kamera ile g�r�nt� alma
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cout << "Kamera a��lamad�!" << std::endl;
        return -1;
    }

    cv::Mat frame;
    while (cap.read(frame)) {
        // Y�zleri alg�la
        std::vector<cv::Rect> faces;
        face_cascade.detectMultiScale(frame, faces, 1.1, 3, 0, cv::Size(30, 30));

        // Ekran�n merkezini hesapla
        cv::Point frameCenter = getFrameCenter(frame);

        // Her y�z i�in pozisyon kontrol� ve uyar�
        for (const auto& face : faces) {
            checkFacePosition(frame, face, frameCenter);
        }

        // Sonu�lar� g�ster
        cv::imshow("Y�z Uyar� Sistemi", frame);
        if (cv::waitKey(30) >= 0) break;
    }

    return 0;
}