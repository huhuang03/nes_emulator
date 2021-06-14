//
// Created by huhua on 2021/6/8.
//

#ifndef NES_PALETTE_H
#define NES_PALETTE_H

#include "../olcPixelGameEngine.h"

class PPU;

/**
 * So a palette is a `device` in the ppu bus.
 */
class Palette {
public:
    Palette();
    const uint16_t addr_min = 0x3f00;
    const uint16_t addr_max = 0x3f1f;
    const uint16_t mirror_min = addr_min;
    const uint16_t mirror_max = 0x3fff;
    uint8_t data[0x20] = {0};

private:
    // pal look like color table.
    // | olc::Pixel - Represents a 32-Bit RGBA colour                                 |
    // it's ppu's color table.
    // why there are 64 'different' color??
    olc::Pixel palScreen[0x40];
    PPU *ppu = nullptr;

public:
    void setPPU(PPU *pPpu);
    uint8_t read(uint16_t addr, bool grayscale);
    void write(uint16_t addr, uint8_t data);
//    olc::Pixel getColor(int palette, int index);
    olc::Pixel getColor(int index);
};


#endif //NES_PALETTE_H
