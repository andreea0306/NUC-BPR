//
// Created by andreea on 28.09.2021.
//

#include <fstream>
#include <opencv2/opencv.hpp>
#include "frame.h"

std::vector<uint16_t> frame::framesToFrame(std::vector<std::string> &v) {
    std::vector<uint32_t> frames;
    frames.insert(frames.begin(), frameHeight * frameWidth, 0);
    std::vector<uint16_t> vect;
    vect.insert(vect.begin(), frameWidth * frameHeight, 0);
    for (auto &i: v) {
        auto buff = new uint16_t[frameWidth * frameHeight];
        std::ifstream in(i, std::ios::binary);
        in.read(reinterpret_cast<char *>(buff), frameHeight * frameWidth * sizeof(uint16_t));
        for (int j = 0; j < frameWidth * frameHeight; j++) {
            frames[j] += (buff[j]);
        }
    }
    for (int j = 0; j < frameWidth * frameHeight; j++) {
        vect[j] = frames[j] / v.size();
    }
    return vect;
}

void frame::getImage(std::vector<uint16_t> v, std::string name) {
    cv::Mat image = cv::Mat(frameHeight, frameWidth, CV_16UC1, v.data());

//    std::vector<uint8_t > newVector;
//    newVector.insert(newVector.begin(), frameWidth*frameHeight,0);
//    for(int i=0;i<frameWidth*frameHeight;i++){
//        newVector[i] = (uint8_t)(v[i]);
//    }
//    cv::Mat newImage = cv::Mat(frameHeight, frameWidth, CV_8UC1, newVector.data());
    //image.convertTo(image, CV_8UC1);
    //cv::medianBlur(image, image, 5);
//    cv::Mat img;
//    cv::bilateralFilter(newImage, img, 5, 80, 80);
    cv::normalize(image, image, 0., 255., cv::NORM_MINMAX, CV_8UC1);

    cv::equalizeHist(image, image);
//    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
//    clahe->setClipLimit(4);
//    clahe->apply(image, image);
    cv::imshow(name, image);
    cv::imwrite("/home/andreea/Desktop/test/" + name + ".png", image);
    //cv::waitKey(0);
}

double frame::pixelsMean(std::vector<uint16_t> &frame) {
    double pixelMean = 0;
    for (int i = 0; i < frameHeight * frameWidth; i++) {
        pixelMean += (frame[i]);
    }
    pixelMean = pixelMean / static_cast<double >(frame.size());
    return pixelMean;
}