#include <opencv2/opencv.hpp>
#include <iostream>

// Yüzün parlaklýðýný analiz eden fonksiyon
void checkFaceBrightness(const cv::Mat& frame, const cv::Rect& face) {
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
        // Eðer yüz bölgesi çok parlaksa uyarý verin
        std::cout << "Uyarý: Yüz çok parlak!" << std::endl;
        cv::putText(frame, "Uyari: Yuz cok parlak!", cv::Point(face.x, face.y - 10),
            cv::FONT_HERSHEY_SIMPLEX, 0.9, cv::Scalar(0, 0, 255), 2);
        cv::rectangle(frame, face, cv::Scalar(0, 0, 255), 2);  // Yüz kutusunu kýrmýzý yap
    }
    else {
        // Yüz parlaklýk seviyesi normalse
        cv::rectangle(frame, face, cv::Scalar(0, 255, 0), 2);  // Yüz kutusunu yeþil yap
    }
}

// Yüzün merkezde olup olmadýðýný kontrol eden fonksiyon
void checkFacePosition(const cv::Mat& frame, const cv::Rect& face) {
    // Yüzün merkezini hesaplayýn
    cv::Point faceCenter(face.x + face.width / 2, face.y + face.height / 2);

    // Görüntünün merkezini hesaplayýn
    cv::Point frameCenter(frame.cols / 2, frame.rows / 2);

    // Yüzün merkezden ne kadar uzak olduðunu hesaplayýn
    int offsetX = std::abs(faceCenter.x - frameCenter.x);
    int offsetY = std::abs(faceCenter.y - frameCenter.y);

    // Eþik deðerlerini belirleyin
    int positionThresholdX = 80;  // Yatay eksende kabul edilen maksimum sapma
    int positionThresholdY = 80;  // Dikey eksende kabul edilen maksimum sapma

    // Eðer yüz ekranda merkezde deðilse uyarý verin
    if (offsetX > positionThresholdX || offsetY > positionThresholdY) {
        std::cout << "Uyarý: Yüz ekranda merkezde deðil!" << std::endl;
        cv::putText(frame, "Uyari: Yuz merkezde degil!", cv::Point(face.x, face.y + face.height + 30),
            cv::FONT_HERSHEY_SIMPLEX, 0.9, cv::Scalar(255, 0, 0), 2);
        cv::rectangle(frame, face, cv::Scalar(255, 0, 0), 2);  // Yüz kutusunu mavi yap
    }
}

int main() {
    // Yüz tespiti için Haar Cascade yükleme
    cv::CascadeClassifier face_cascade;
    face_cascade.load("C:/Users/Okutgen/Downloads/haarcascade_frontalface_default.xml");

    // Kamera ile görüntü alma
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Kamera açýlamadý!" << std::endl;
        return -1;
    }

    cv::Mat frame;
    while (cap.read(frame)) {
        // Yüzleri algýla
        std::vector<cv::Rect> faces;
        face_cascade.detectMultiScale(frame, faces, 1.1, 3, 0, cv::Size(30, 30));

        // Her yüz için parlaklýk ve konum kontrolü
        for (const auto& face : faces) {
            checkFaceBrightness(frame, face);  // Parlaklýk kontrolü
            checkFacePosition(frame, face);    // Konum kontrolü
        }

        // Sonuçlarý göster
        cv::imshow("Yüz Parlaklýk ve Konum Uyarýsý", frame);

        // 'q' tuþuna basýldýðýnda döngüyü sonlandýr
        if (cv::waitKey(30) == 'q') break;
    }

    return 0;
}