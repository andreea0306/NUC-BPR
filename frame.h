//
// Created by andreea on 28.09.2021.
//

#pragma once

#include <vector>
#include <string>

static const int frameWidth = 1280;
static const int frameHeight = 720;

class frame {
public:
    static std::vector<uint16_t> framesToFrame(std::vector<std::string> &);

    static void getImage(std::vector<uint16_t>, std::string);

    static double pixelsMean(std::vector<uint16_t> &);
};

