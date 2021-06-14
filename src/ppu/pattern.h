//
// Created by huhua on 2021/6/7.
//

#ifndef NES_PATTERN_H
#define NES_PATTERN_H


#include <cstdint>
#include "olcPixelGameEngine.h"

class PPU;

/**
 * What is a pattern?
 * A pattern is some address lined to ppu bus.
 *
 * And it's address in ppu is 0x0 - 0x1fff
 *
 * In virtual it's spite to too table [0x0 - 0xfff], [0x1000 - 0x1fff]
 *
 * A table is consist of 16 x 16 tile.
 * A tile is consist of 8 x 8 pixel
 * A pixel need two bit. But the tile is something like this:
 * First the first bit of 8 pixel, Then the second bit of 8
 * . . . . . . . .                 . . . . . . . .
 * Followed by the other 7 line of pixel
 *
 */
class Pattern {
private:
    // this is all single table value.
    const uint8_t width = 128;
    const uint8_t height = 128;
    const uint8_t num_tile = 16;
    const uint8_t num_pixel_in_tile = 8;

    uint32_t min = 0x0;
    uint32_t max = 0x1fff;

    uint32_t table1_min = min;
    uint32_t table1_max = 0xfff;

    uint32_t table2_min = 0x1000;
    uint32_t table2_max = 0x1fff;

    // mostly as the bg?
    olc::Sprite sprite1 = olc::Sprite(width, height);
    olc::Sprite sprite2 = olc::Sprite(width, height);

    olc::Sprite* sprites[2] = {&sprite1, &sprite2};
//    uint8_t data[0x1fff] = {0};
    PPU *ppu = nullptr;

public:
    explicit Pattern() = default;;
    void setPPU(PPU *ppu);
    olc::Sprite& getSprite(int which, int palette);
};


#endif //NES_PATTERN_H
