//
// Created by huhua on 2021/6/8.
//

#ifndef NES_PALETTE_H
#define NES_PALETTE_H

#include "../olcPixelGameEngine.h"

namespace th {
    class PPU;

/**
 * https://wiki.nesdev.com/w/index.php/PPU_palettes
 *
 * the unit is one byte.
 * so there are 0x20 byte which mens 32 value.
 *
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
        // A 6-bit value in the palette memory area corresponds to one of 64 outputs
        olc::Pixel palScreen[0x40];
        PPU *ppu = nullptr;

        uint16_t mirror(uint16_t addr);


    public:
        void setPPU(PPU *pPpu);

        uint8_t read(uint16_t addr, bool grayscale);

        void write(uint16_t addr, uint8_t data);

        olc::Pixel getColor(int index);

        olc::Pixel getColor(int which, int index);
    };

}
#endif //NES_PALETTE_H
