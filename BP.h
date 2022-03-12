//
// Created by andreea on 28.09.2021.
//

#pragma once

#include <vector>
#include <string>

class BP {
public:
    static std::vector<uint16_t> offsetFrame(std::vector<std::string>);

    static double meanOffset(std::vector<uint16_t>);

    static std::vector<bool> isBadPixel(std::vector<uint16_t>, double);
};



