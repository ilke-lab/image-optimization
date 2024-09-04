#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

// Yüzün parlaklýðýný analiz eden fonksiyon
void checkFaceBrightness(const cv::Mat& frame, const cv::Rect& face, double fontScale, cv::Point textPosition) {
    // Yüz bölgesini kýrpýn
    cv::Mat faceROI = frame(face);

    // Yüz bölgesini gri tonlamaya çevirin
    cv::Mat grayFace;
    cv::cvtColor(faceROI, grayFace, cv::COLOR_BGR2GRAY);

    // Yüz bölgesinin ortalama parlaklýðýný hesaplayýn
    cv::Scalar meanBrightness = cv::mean(grayFace);

    // Parlaklýk eþiðini belirleyin
    double brightnessThreshold = 150.0;

    if (meanBrightness[0] > brightnessThreshold) {
        std::string warning = "Uyarý: Yüz çok parlak!";
        std::cout << warning << std::endl;

        // Yazý kalýnlýðý
        int thickness = 2;
        cv::Scalar textColor(0, 0, 255);  // Kýrmýzý renk

        // Metni belirtilen konumda göster
        cv::putText(frame, "Uyari: Yuz cok parlak!", textPosition,
            cv::FONT_HERSHEY_SIMPLEX, fontScale, textColor, thickness);

        // Yüzü çevreleyen çerçeveyi kýrmýzý yap (parlaklýk uyarýsý için)
        cv::rectangle(frame, face, cv::Scalar(0, 0, 255), 2);
    }
    else {
        // Yüzü çevreleyen çerçeveyi yeþil yap (parlaklýk normal)
        cv::rectangle(frame, face, cv::Scalar(0, 255, 0), 2);
    }
}

// Yüzün merkezde olup olmadýðýný kontrol eden fonksiyon
void checkFacePosition(const cv::Mat& frame, const cv::Rect& face, double fontScale, cv::Point textPosition) {
    cv::Point faceCenter(face.x + face.width / 2, face.y + face.height / 2);
    cv::Point frameCenter(frame.cols / 2, frame.rows / 2);

    int offsetX = std::abs(faceCenter.x - frameCenter.x);
    int offsetY = std::abs(faceCenter.y - frameCenter.y);

    int positionThresholdX = 80;  // Yatay eksende kabul edilen maksimum sapma
    int positionThresholdY = 100;  // Dikey eksende kabul edilen maksimum sapma

    if (offsetX > positionThresholdX || offsetY > positionThresholdY) {
        std::string warning = "Uyarý: Yüz ekranda merkezde deðil!";
        std::cout << warning << std::endl;

        // Yazý kalýnlýðý
        int thickness =2;
        cv::Scalar textColor(255, 0, 0);  // Mavi renk

        // Metni belirtilen konumda göster
        cv::putText(frame, "Uyari: Yuz merkezde degil!", textPosition,
            cv::FONT_HERSHEY_SIMPLEX, fontScale, textColor, thickness);

        // Yüzü çevreleyen çerçeveyi mavi yap (konum uyarýsý)
        cv::rectangle(frame, face, cv::Scalar(255, 0, 0), 2);
    }
}

// Yüzün kameraya olan uzaklýðýný kontrol eden fonksiyon
void checkFaceDistance(const cv::Mat& frame, const cv::Rect& face, double fontScale, cv::Point textPosition) {
    int faceHeight = face.height;

    int nearThreshold = 240;
    int farThreshold = 150;

    if (faceHeight > nearThreshold) {
        std::string warning = "Uyarý: Yüze çok yakýnsýnýz!";
        std::cout << warning << std::endl;

        // Yazý kalýnlýðý
        int thickness = 2;
        cv::Scalar textColor(0, 255, 255);  // Sarý renk

        // Metni belirtilen konumda göster
        cv::putText(frame, "Uyari: Ekrana cok yakinsiniz!", textPosition,
            cv::FONT_HERSHEY_SIMPLEX, fontScale, textColor, thickness);

        // Yüzü çevreleyen çerçeveyi sarý yap (uzaklýk uyarýsý)
        cv::rectangle(frame, face, cv::Scalar(0, 255, 255), 2);
    }
    else if (faceHeight < farThreshold) {
        std::string warning = "Uyarý: Yüz çok uzakta!";
        std::cout << warning << std::endl;

        // Yazý kalýnlýðý
        int thickness = 2;
        cv::Scalar textColor(0, 255, 255);  // Sarý renk

        // Metni belirtilen konumda göster
        cv::putText(frame, "Uyari: Yuz cok uzakta!", textPosition,
            cv::FONT_HERSHEY_SIMPLEX, fontScale, textColor, thickness);

        // Yüzü çevreleyen çerçeveyi sarý yap (uzaklýk uyarýsý)
        cv::rectangle(frame, face, cv::Scalar(0, 255, 255), 2);
    }
}

int main() {
    cv::CascadeClassifier face_cascade;
    face_cascade.load("C:/Users/Okutgen/Downloads/haarcascade_frontalface_default.xml");

    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Kamera açýlamadý!" << std::endl;
        return -1;
    }

    cv::Mat frame;
    while (cap.read(frame)) {
        std::vector<cv::Rect> faces;
        face_cascade.detectMultiScale(frame, faces, 1.1, 3, 0, cv::Size(30, 30));

        // Belirli bir yazý boyutu ve konum ayarlama
        double fontScale = 0.5;  // Yazý boyutu
        cv::Point textPosition(80, 50);  // Yazýnýn ekranda gösterileceði yer (x, y)

        for (const auto& face : faces) {
            checkFaceBrightness(frame, face, fontScale, textPosition);  // Parlaklýk kontrolü
            checkFacePosition(frame, face, fontScale, textPosition);    // Konum kontrolü
            checkFaceDistance(frame, face, fontScale, textPosition);    // Uzaklýk kontrolü
        }

        cv::imshow("Yüz Parlaklýk, Konum ve Uzaklýk Uyarýsý", frame);

        if (cv::waitKey(30) == 'q') break;
    }

    return 0;
}
