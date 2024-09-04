#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>

// Yüzün merkezini hesaplayan fonksiyon
cv::Point calculateFaceCenter(const cv::Rect& face) {
    return cv::Point(face.x + face.width / 2, face.y + face.height / 2);
}

// Çerçevenin merkezini döndüren fonksiyon
cv::Point getFrameCenter(const cv::Mat& frame) {
    return cv::Point(frame.cols / 2, frame.rows / 2);
}

// Ýki nokta arasýndaki mesafeyi hesaplayan fonksiyon
double calculateDistance(const cv::Point& p1, const cv::Point& p2) {
    return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
}

// Yüzün konumunu kontrol eden ve uyarý veren fonksiyon
void checkFacePosition(cv::Mat& frame, const cv::Rect& face, const cv::Point& frameCenter) {
    cv::Point faceCenter = calculateFaceCenter(face);
    double distance = calculateDistance(faceCenter, frameCenter);

    double threshold = 100.0; // Mesafe eþiði

    if (distance > threshold) {
        std::string warningText = "Yüz merkezde deðil!";
        cv::putText(frame, warningText, cv::Point(50, 50), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2);
        cv::rectangle(frame, face, cv::Scalar(0, 0, 255), 2); // Yüzü kýrmýzý kutu ile göster
    }
    else {
        cv::rectangle(frame, face, cv::Scalar(0, 255, 0), 2); // Yüzü yeþil kutu ile göster
    }
}

int main() {
    // Yüz tespiti için Haar Cascade yükleme
    cv::CascadeClassifier face_cascade;
    face_cascade.load("C:/Users/Okutgen/Downloads/haarcascade_frontalface_default.xml");

    // Kamera ile görüntü alma
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cout << "Kamera açýlamadý!" << std::endl;
        return -1;
    }

    cv::Mat frame;
    while (cap.read(frame)) {
        // Yüzleri algýla
        std::vector<cv::Rect> faces;
        face_cascade.detectMultiScale(frame, faces, 1.1, 3, 0, cv::Size(30, 30));

        // Ekranýn merkezini hesapla
        cv::Point frameCenter = getFrameCenter(frame);

        // Her yüz için pozisyon kontrolü ve uyarý
        for (const auto& face : faces) {
            checkFacePosition(frame, face, frameCenter);
        }

        // Sonuçlarý göster
        cv::imshow("Yüz Uyarý Sistemi", frame);
        if (cv::waitKey(30) >= 0) break;
    }

    return 0;
}