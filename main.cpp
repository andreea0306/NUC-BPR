#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include "frame.h"
#include <fstream>
#include "BP.h"

int main() {
    std::vector<std::string> coldFrameNames;
    std::vector<std::string> warmFrameNames;
    for (int i = 0; i < 50; i++) {
        coldFrameNames.push_back("/home/andreea/Desktop/black_body/cold/frame_0_" + std::to_string(i) + ".raw");
        warmFrameNames.push_back("/home/andreea/Desktop/black_body/warm/frame_1_" + std::to_string(i) + ".raw");
    }

    std::vector<uint16_t> coldFrame, warmFrame;
    coldFrame.insert(coldFrame.begin(), frameHeight * frameWidth, 0);
    warmFrame.insert(warmFrame.begin(), frameHeight * frameWidth, 0);
    coldFrame = frame::framesToFrame(coldFrameNames);
    warmFrame = frame::framesToFrame(warmFrameNames);
//    frame::getImage(coldFrame,0);
//    frame::getImage(warmFrame,1);
//    cv::waitKey(0);
    double coldPixelMean, warmPixelMean;
    coldPixelMean = frame::pixelsMean(coldFrame);
    warmPixelMean = frame::pixelsMean(warmFrame);
    //std::cout<<coldPixelMean<<" "<<warmPixelMean; // coldPixelMean = 1290 warmPixelMean = 891
    std::vector<double> gains, offsets;
    gains.insert(gains.begin(), frameWidth * frameHeight, 0);
    offsets.insert(offsets.begin(), frameWidth * frameHeight, 0);
    for (int i = 0; i < frameHeight * frameWidth; i++) {
        gains[i] = static_cast<double >((warmPixelMean - coldPixelMean) /
                                        static_cast<double >(warmFrame[i] - coldFrame[i]));
        offsets[i] = static_cast<double >(coldPixelMean - gains[i] * static_cast<double >(coldFrame[i]));
        std::cout << gains[i] << " " << offsets[i] << std::endl;
    }
    //std::cout<<gains[0]<< " "<<offsets[0];
    std::vector<double> test;
    test.insert(test.begin(), frameWidth * frameHeight, 0);
    auto buff = new uint16_t[frameHeight * frameWidth];
    std::ifstream in("/home/andreea/Desktop/black_body/frame_test.raw", std::ios::binary);
    in.read(reinterpret_cast<char *>(buff), frameWidth * frameHeight * sizeof(uint16_t));
    for (int k = 0; k < frameHeight * frameWidth; k++) {
        test[k] = static_cast<double>(buff[k]);
    }
    delete[] buff;
    in.close();

    std::vector<std::string> offsetNames;
    for (int i = 0; i < 50; i++) {
        offsetNames.push_back("/home/andreea/Desktop/black_body/frame_test.raw");
    }
    std::vector<uint16_t> offsetFrame;
    offsetFrame.insert(offsetFrame.begin(), frameWidth * frameHeight, 0);
    offsetFrame = BP::offsetFrame(offsetNames);
    //frame::getImage(offsetFrame,0);
    double meanOffset = 0;
    meanOffset = BP::meanOffset(offsetFrame);
    //std::cout<<meanOffset; // meanOffset = 19093.3
    std::vector<bool> isBadPixel;
    isBadPixel.insert(isBadPixel.begin(), frameHeight * frameWidth, false);
    isBadPixel = BP::isBadPixel(offsetFrame, meanOffset);

    std::vector<double> test2;
    test2.insert(test2.begin(), frameWidth * frameHeight, 0);
    for (int i = 0; i < frameHeight * frameWidth; i++) {
        if (isBadPixel[i]) {
            if (i == 0) {
                test2[i] = (test[i + 1] + test[i + frameWidth] + test[i + frameWidth + 1]) / 3;
                //std::cout<<frame[i]<<std::endl;
            } else if (i > 0 && i < (frameWidth - 1)) {
                test2[i] = (test[i - 1] + test[i + 1] + test[i + frameWidth - 1] + test[i + frameWidth] +
                            test[i + frameWidth + 1]) / 5;
            } else if (i == (frameWidth - 1)) {
                test2[i] = (test[i - 1] + test[i + frameWidth - 1] + test[i + frameWidth]) / 3;
            } else if (i % frameWidth == 0) {
                test2[i] = (test[i - frameWidth] + test[i - frameWidth + 1] + test[i + 1] + test[i + frameWidth] +
                            test[i + frameWidth + 1]) / 5;
            } else if (i == ((frameHeight - 1) * frameWidth)) {
                test2[i] = (test[i - frameWidth] + test[i - frameWidth + 1] + test[i + 1]) / 3;
            } else if (i > ((frameHeight - 1) * frameWidth) && i < (frameWidth * frameHeight - 1)) {
                test2[i] = (test[i - frameWidth - 1] + test[i - frameWidth] + test[i - frameWidth + 1] + test[i - 1] +
                            test[i + 1]) / 5;
            } else if (i == (frameHeight * frameWidth - 1)) {
                test2[i] = (test[i - frameWidth - 1] + test[i - frameWidth] + test[i - 1]) / 3;
            } else if (i % frameWidth == (frameWidth - 1)) {
                test2[i] = (test[i - frameWidth - 1] + test[i - frameWidth] + test[i - 1] + test[i + frameWidth - 1] +
                            test[i + frameWidth]) / 5;
            } else {
                test2[i] = (test[i - frameWidth - 1] + test[i - frameWidth] + test[i - frameWidth + 1] + test[i - 1] +
                            test[i + 1] + test[i + frameWidth - 1] + test[i + frameWidth] + test[i + frameWidth + 1]) /
                           8;
            }
        } else {
            test2[i] = (test[i] * gains[i] + offsets[i]);
            //std::cout<<test2[k]<<" ";
        }
    }
    std::vector<uint16_t> testImage;
    testImage.insert(testImage.begin(), frameHeight * frameWidth, 0);
    for (int i = 0; i < frameWidth * frameHeight; i++) {
        testImage[i] = static_cast<uint16_t>(test2[i]);
    }
    frame::getImage(testImage, "NUC+BP testimgage");
    cv::waitKey(0);


//    for(int i=0;i<frameHeight*frameWidth;i++){
//        if(isBadPixel[i]){
//            std::cout<<test2[i]<<" "<<isBadPixel[i]<<std::endl;
//        }
//    }

//    double test = warmPixelMean - coldPixelMean;
//    std::cout<<test;
//    double  test2;
//    for(int i=0;i<frameHeight*frameWidth;i++){
//        test2 = warmFrame[i] - coldFrame[i];
//        std::cout<<test2<<" ";
//    }

    return 0;
}
