#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Header.h"
#include <iomanip>

using namespace cv;
using namespace std;

const string ASCII_CHARS = "@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/|()1{}[]?-_+~<>i!lI;:,^`\"'.";


Mat resizeImage(Mat image, int new_width) {
    float aspect_ratio = (float)image.rows / image.cols;
    int new_height = aspect_ratio * new_width;
    Mat new_image;
    resize(image, new_image, Size(new_width, new_height));
    return new_image;
}




// ASCII
string imageToAscii(Mat image) {
    cvtColor(image, image, COLOR_BGR2GRAY);
    string ascii_image = "";
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            uchar pixel_value = image.at<uchar>(i, j);
            ascii_image += ASCII_CHARS[pixel_value / 4];
        }
        ascii_image += "\n";
    }
    return ascii_image;
}

std::string base64_encode(const std::vector<uchar>& data) {
    return { data.begin(), data.end() };
}

std::vector<uchar> base64_decode(const std::string& encodedData) {
    return { encodedData.begin(), encodedData.end() };
}

void printBase64Image(const std::string& encodedImage) {
    
    const int width = 80;
    for (size_t i = 0; i < encodedImage.size(); i += width) {
        std::cout << encodedImage.substr(i, width) << std::endl;
    }
}

void applySepiaEffect(Mat& image) {
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            Vec3b& pixel = image.at<Vec3b>(i, j);
            int sepiaIntensity = 0.393 * pixel[2] + 0.769 * pixel[1] + 0.189 * pixel[0];
            int sepiaBlue = (sepiaIntensity < 255) ? sepiaIntensity : 255;
            sepiaIntensity = 0.349 * pixel[2] + 0.686 * pixel[1] + 0.168 * pixel[0];
            int sepiaGreen = (sepiaIntensity < 255) ? sepiaIntensity : 255;
            sepiaIntensity = 0.272 * pixel[2] + 0.534 * pixel[1] + 0.131 * pixel[0];
            int sepiaRed = (sepiaIntensity < 255) ? sepiaIntensity : 255;
            pixel[0] = sepiaBlue;
            pixel[1] = sepiaGreen;
            pixel[2] = sepiaRed;
        }
    }
}

void applyBlackAndWhiteEffect(Mat& image) {
    cvtColor(image, image, COLOR_BGR2GRAY);
    threshold(image, image, 127, 255, THRESH_BINARY);
}

// Эффект "Шум"
void applyNoiseEffect(Mat& image) {
    Mat noise(image.size(), CV_8UC3); // шум для трех каналов
    randn(noise, 0, 25); // ШуМ
    addWeighted(image, 1.0, noise, 1.0, 0.0, image);
}


void applyNegativeEffect(Mat& image) {
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            Vec3b& pixel = image.at<Vec3b>(i, j);
            pixel[0] = 255 - pixel[0]; // к
            pixel[1] = 255 - pixel[1]; // з
            pixel[2] = 255 - pixel[2]; // с
        }
    }
}

void applyBWEffect(Mat& image){
    for(int i = 0; i < image.rows; i++){
        for(int j = 0; j < image.cols; j++){
            
        }
    }
}

int main(int argc, char** argv) {
    setlocale(LC_ALL, "ru");
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << "/Users/ayzeksalimli/Desktop/Ayzek/image.jpg" << std::endl;
        return 1;
    }

    std::string imagePath = argv[1];

    cv::Mat image = cv::imread(imagePath);
    if (image.empty()) {
        std::cerr << "Unable to open image file." << std::endl;
        return 1;
    }

    
    std::vector<uchar> buf;
    cv::imencode(".jpg", image, buf);

    std::string encodedImage = base64_encode(buf);

    
    std::cout << "Encoded Image (Base64):" << std::endl;
    printBase64Image(encodedImage);

    
    std::vector<uchar> decodedBuf = base64_decode(encodedImage);

    cv::Mat decodedImage = cv::imdecode(decodedBuf, cv::IMREAD_COLOR);

    
    cv::imwrite("/Users/ayzeksalimli/Desktop/Ayzek/decoded_image.jpg", decodedImage);
    if (buf == decodedBuf) {
        std::cout << "Ура, сработало!." << std::endl;
        // Вычисление коэффициента сжатия
        double k = static_cast<double>(decodedBuf.size()) / encodedImage.size();
        std::cout << "Коэффициент сжатия k = " << k << std::endl;
    } else {
        std::cout << "Не сработало(." << std::endl;
    }

    
    cout << endl;
    cout << endl;
    cout << endl;
    
    
        applySepiaEffect(image);
        string sepia_output_path = "/Users/ayzeksalimli/Desktop/Ayzek/sepia_image.jpg";
        imwrite(sepia_output_path, image);

        
        Mat image_bw = imread(imagePath);
        if (image_bw.empty()) {
            cout << "Ошибка при загрузке изображения!" << endl;
            return -1;
        }
        applyBlackAndWhiteEffect(image_bw);
        string bw_output_path = "/Users/ayzeksalimli/Desktop/Ayzek/black_and_white_image.jpg";
        imwrite(bw_output_path, image_bw);

        
        Mat image_noise = imread(imagePath);
        if (image_noise.empty()) {
            cout << "Ошибка при загрузке изображения!" << endl;
            return -1;
        }
        applyNoiseEffect(image_noise);
        string noise_output_path = "/Users/ayzeksalimli/Desktop/Ayzek/noise_image.jpg";
        imwrite(noise_output_path, image_noise);
    
       applyNegativeEffect(image);
    string output_path = "/Users/ayzeksalimli/Desktop/Ayzek/negative_image.jpg";
    imwrite(output_path, image);

    
    string image_path = "/Users/ayzeksalimli/Desktop/Ayzek/image.jpg";
      Mat image1 = imread(image_path);
      if (image1.empty()) {
          cout << "Ошибка при загрузке изображения!" << endl;
          return -1;
      }
        
      int new_width = 100; //100
      Mat resized_image = resizeImage(image, new_width);
      string ascii_image = imageToAscii(resized_image);
        
      cout << ascii_image << endl;

      waitKey(0);
      destroyAllWindows();
    return 0;
}
