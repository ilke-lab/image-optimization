#include <opencv2/opencv.hpp>
#include <iostream>

// Y�z�n parlakl���n� analiz eden fonksiyon
void checkFaceBrightness(const cv::Mat& frame, const cv::Rect& face) {
    // Y�z b�lgesini k�rp�n
    cv::Mat faceROI = frame(face);

    // Y�z b�lgesini gri tonlamaya �evirin
    cv::Mat grayFace;
    cv::cvtColor(faceROI, grayFace, cv::COLOR_BGR2GRAY);

    // Y�z b�lgesinin ortalama parlakl���n� hesaplay�n
    cv::Scalar meanBrightness = cv::mean(grayFace);

    // Parlakl�k e�i�ini belirleyin
    double brightnessThreshold = 150.0;

    if (meanBrightness[0] > brightnessThreshold) {
        // E�er y�z b�lgesi �ok parlaksa uyar� verin
        std::cout << "Uyar�: Y�z �ok parlak!" << std::endl;
        cv::putText(frame, "Uyari: Yuz cok parlak!", cv::Point(face.x, face.y - 10),
            cv::FONT_HERSHEY_SIMPLEX, 0.9, cv::Scalar(0, 0, 255), 2);
        cv::rectangle(frame, face, cv::Scalar(0, 0, 255), 2);  // Y�z kutusunu k�rm�z� yap
    }
    else {
        // Y�z parlakl�k seviyesi normalse
        cv::rectangle(frame, face, cv::Scalar(0, 255, 0), 2);  // Y�z kutusunu ye�il yap
    }
}

// Y�z�n merkezde olup olmad���n� kontrol eden fonksiyon
void checkFacePosition(const cv::Mat& frame, const cv::Rect& face) {
    // Y�z�n merkezini hesaplay�n
    cv::Point faceCenter(face.x + face.width / 2, face.y + face.height / 2);

    // G�r�nt�n�n merkezini hesaplay�n
    cv::Point frameCenter(frame.cols / 2, frame.rows / 2);

    // Y�z�n merkezden ne kadar uzak oldu�unu hesaplay�n
    int offsetX = std::abs(faceCenter.x - frameCenter.x);
    int offsetY = std::abs(faceCenter.y - frameCenter.y);

    // E�ik de�erlerini belirleyin
    int positionThresholdX = 80;  // Yatay eksende kabul edilen maksimum sapma
    int positionThresholdY = 80;  // Dikey eksende kabul edilen maksimum sapma

    // E�er y�z ekranda merkezde de�ilse uyar� verin
    if (offsetX > positionThresholdX || offsetY > positionThresholdY) {
        std::cout << "Uyar�: Y�z ekranda merkezde de�il!" << std::endl;
        cv::putText(frame, "Uyari: Yuz merkezde degil!", cv::Point(face.x, face.y + face.height + 30),
            cv::FONT_HERSHEY_SIMPLEX, 0.9, cv::Scalar(255, 0, 0), 2);
        cv::rectangle(frame, face, cv::Scalar(255, 0, 0), 2);  // Y�z kutusunu mavi yap
    }
}

int main() {
    // Y�z tespiti i�in Haar Cascade y�kleme
    cv::CascadeClassifier face_cascade;
    face_cascade.load("C:/Users/Okutgen/Downloads/haarcascade_frontalface_default.xml");

    // Kamera ile g�r�nt� alma
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Kamera a��lamad�!" << std::endl;
        return -1;
    }

    cv::Mat frame;
    while (cap.read(frame)) {
        // Y�zleri alg�la
        std::vector<cv::Rect> faces;
        face_cascade.detectMultiScale(frame, faces, 1.1, 3, 0, cv::Size(30, 30));

        // Her y�z i�in parlakl�k ve konum kontrol�
        for (const auto& face : faces) {
            checkFaceBrightness(frame, face);  // Parlakl�k kontrol�
            checkFacePosition(frame, face);    // Konum kontrol�
        }

        // Sonu�lar� g�ster
        cv::imshow("Y�z Parlakl�k ve Konum Uyar�s�", frame);

        // 'q' tu�una bas�ld���nda d�ng�y� sonland�r
        if (cv::waitKey(30) == 'q') break;
    }

    return 0;
}