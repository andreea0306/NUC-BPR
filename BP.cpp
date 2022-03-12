//
// Created by andreea on 28.09.2021.
//

#include <fstream>
#include "BP.h"
#include "frame.h"

std::vector<bool> BP::isBadPixel(std::vector<uint16_t> frame, double meanOffset) {
    std::vector<bool> isBadPixel;
    isBadPixel.insert(isBadPixel.begin(), frameWidth * frameHeight, false);
    for (int i = 0; i < frameWidth * frameHeight; i++) {
        if (std::abs(frame[i] - meanOffset) > 0.25 * meanOffset) {
            isBadPixel[i] = true;
        } else {
            isBadPixel[i] = false;
        }
    }
    return isBadPixel;
}

double BP::meanOffset(std::vector<uint16_t> offsetFrame) {
    double meanOffset = 0;
    for (int i = 0; i < frameHeight * frameWidth; i++) {
        meanOffset += offsetFrame[i];
    }
    meanOffset /= (offsetFrame.size());
    return meanOffset;
}

std::vector<uint16_t> BP::offsetFrame(std::vector<std::string> frameNames) {
    std::vector<uint64_t> offsetFrame;
    offsetFrame.insert(offsetFrame.begin(), frameHeight * frameWidth, 0);
    for (auto &i: frameNames) {
        auto buff = new uint16_t[frameWidth * frameHeight];
        std::ifstream in(i, std::ios::binary);
        in.read(reinterpret_cast<char *>(buff), frameHeight * frameWidth * sizeof(uint16_t));
        for (int j = 0; j < frameWidth * frameHeight; j++) {
            offsetFrame[j] += (buff[j]);
        }
        delete[] buff;
        in.close();
    }
    std::vector<uint16_t> offsetFrame2;
    offsetFrame2.insert(offsetFrame2.begin(), frameHeight * frameWidth, 0);
    for (int i = 0; i < frameWidth * frameHeight; i++) {
        offsetFrame2[i] = offsetFrame[i] / frameNames.size();
    }
    return offsetFrame2;
}



