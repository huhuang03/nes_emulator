//
// Created by huhua on 2021/6/7.
//

#ifndef NES_PATTERN_H
#define NES_PATTERN_H


#include <cstdint>
#include "olcPixelGameEngine.h"

/**
 * Can split to 0x0 - 0xfff, 0x1000 - 0x1fff two table
 */
class Pattern {
private:
    uint32_t min = 0x0;
    uint32_t max = 0x1fff;

    uint32_t table1_min = min;
    uint32_t table1_max = 0xfff;

    uint32_t table2_min = 0x1000;
    uint32_t table2_max = 0x1fff;


    olc::Pixel& getTable(int which);
};


#endif //NES_PATTERN_H
