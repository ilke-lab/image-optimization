#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

// Y�z�n parlakl���n� analiz eden fonksiyon
void checkFaceBrightness(const cv::Mat& frame, const cv::Rect& face, double fontScale, cv::Point textPosition) {
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
        std::string warning = "Uyar�: Y�z �ok parlak!";
        std::cout << warning << std::endl;

        // Yaz� kal�nl���
        int thickness = 2;
        cv::Scalar textColor(0, 0, 255);  // K�rm�z� renk

        // Metni belirtilen konumda g�ster
        cv::putText(frame, "Uyari: Yuz cok parlak!", textPosition,
            cv::FONT_HERSHEY_SIMPLEX, fontScale, textColor, thickness);

        // Y�z� �evreleyen �er�eveyi k�rm�z� yap (parlakl�k uyar�s� i�in)
        cv::rectangle(frame, face, cv::Scalar(0, 0, 255), 2);
    }
    else {
        // Y�z� �evreleyen �er�eveyi ye�il yap (parlakl�k normal)
        cv::rectangle(frame, face, cv::Scalar(0, 255, 0), 2);
    }
}

// Y�z�n merkezde olup olmad���n� kontrol eden fonksiyon
void checkFacePosition(const cv::Mat& frame, const cv::Rect& face, double fontScale, cv::Point textPosition) {
    cv::Point faceCenter(face.x + face.width / 2, face.y + face.height / 2);
    cv::Point frameCenter(frame.cols / 2, frame.rows / 2);

    int offsetX = std::abs(faceCenter.x - frameCenter.x);
    int offsetY = std::abs(faceCenter.y - frameCenter.y);

    int positionThresholdX = 80;  // Yatay eksende kabul edilen maksimum sapma
    int positionThresholdY = 100;  // Dikey eksende kabul edilen maksimum sapma

    if (offsetX > positionThresholdX || offsetY > positionThresholdY) {
        std::string warning = "Uyar�: Y�z ekranda merkezde de�il!";
        std::cout << warning << std::endl;

        // Yaz� kal�nl���
        int thickness =2;
        cv::Scalar textColor(255, 0, 0);  // Mavi renk

        // Metni belirtilen konumda g�ster
        cv::putText(frame, "Uyari: Yuz merkezde degil!", textPosition,
            cv::FONT_HERSHEY_SIMPLEX, fontScale, textColor, thickness);

        // Y�z� �evreleyen �er�eveyi mavi yap (konum uyar�s�)
        cv::rectangle(frame, face, cv::Scalar(255, 0, 0), 2);
    }
}

// Y�z�n kameraya olan uzakl���n� kontrol eden fonksiyon
void checkFaceDistance(const cv::Mat& frame, const cv::Rect& face, double fontScale, cv::Point textPosition) {
    int faceHeight = face.height;

    int nearThreshold = 240;
    int farThreshold = 150;

    if (faceHeight > nearThreshold) {
        std::string warning = "Uyar�: Y�ze �ok yak�ns�n�z!";
        std::cout << warning << std::endl;

        // Yaz� kal�nl���
        int thickness = 2;
        cv::Scalar textColor(0, 255, 255);  // Sar� renk

        // Metni belirtilen konumda g�ster
        cv::putText(frame, "Uyari: Ekrana cok yakinsiniz!", textPosition,
            cv::FONT_HERSHEY_SIMPLEX, fontScale, textColor, thickness);

        // Y�z� �evreleyen �er�eveyi sar� yap (uzakl�k uyar�s�)
        cv::rectangle(frame, face, cv::Scalar(0, 255, 255), 2);
    }
    else if (faceHeight < farThreshold) {
        std::string warning = "Uyar�: Y�z �ok uzakta!";
        std::cout << warning << std::endl;

        // Yaz� kal�nl���
        int thickness = 2;
        cv::Scalar textColor(0, 255, 255);  // Sar� renk

        // Metni belirtilen konumda g�ster
        cv::putText(frame, "Uyari: Yuz cok uzakta!", textPosition,
            cv::FONT_HERSHEY_SIMPLEX, fontScale, textColor, thickness);

        // Y�z� �evreleyen �er�eveyi sar� yap (uzakl�k uyar�s�)
        cv::rectangle(frame, face, cv::Scalar(0, 255, 255), 2);
    }
}

int main() {
    cv::CascadeClassifier face_cascade;
    face_cascade.load("C:/Users/Okutgen/Downloads/haarcascade_frontalface_default.xml");

    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Kamera a��lamad�!" << std::endl;
        return -1;
    }

    cv::Mat frame;
    while (cap.read(frame)) {
        std::vector<cv::Rect> faces;
        face_cascade.detectMultiScale(frame, faces, 1.1, 3, 0, cv::Size(30, 30));

        // Belirli bir yaz� boyutu ve konum ayarlama
        double fontScale = 0.5;  // Yaz� boyutu
        cv::Point textPosition(80, 50);  // Yaz�n�n ekranda g�sterilece�i yer (x, y)

        for (const auto& face : faces) {
            checkFaceBrightness(frame, face, fontScale, textPosition);  // Parlakl�k kontrol�
            checkFacePosition(frame, face, fontScale, textPosition);    // Konum kontrol�
            checkFaceDistance(frame, face, fontScale, textPosition);    // Uzakl�k kontrol�
        }

        cv::imshow("Y�z Parlakl�k, Konum ve Uzakl�k Uyar�s�", frame);

        if (cv::waitKey(30) == 'q') break;
    }

    return 0;
}
