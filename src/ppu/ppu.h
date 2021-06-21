//
// Created by hwf on 2021/6/4.
//

#ifndef NES_PPU_H
#define NES_PPU_H


#include <cstdint>
#include <memory>
#include "cartridge.h"

#include "./olcPixelGameEngine.h"
#include "palette.h"
#include "pattern.h"
#include "./bridge.h"

// model: 2C02
/**
 * We use pixel game engine to draw the game.
 *
 * 0x0000 - 0x1FFF Pattern Memory "CHR ROM"
 *
 * 0x2000 - 0x3EFF Name Table Memory
 * 0x3F00 - 0x3FFF Palette Memory
 *
 * * Pattern
 * We have two pattern table in 0x0000 - 0x0fff - 0x1fff
 * A pixel is 2 bit
 * A table is 4k = 128pixel * 128pixel = 16 x 16tiles
 * A Tile = 8 * 8 pixel
 *
 * * Tile:
 * A Tile is 8x8 pixels. A pixel can only have 4 value
 */
class PPU {
    // Now, let's remove some magic
private:
    const uint16_t scan_width = 341;
    const uint16_t scan_height = 261;
    const uint16_t width = 256;
    const uint16_t height = 240;

public:
    PPU();
    /**
     * This is called by cpu, and when read cpu range 0x2000 - 0x3ffff, is read to ppu.
     */
    uint8_t cpuRead(uint16_t addr, bool readOnly = false);
    void cpuWrite(uint16_t addr, uint8_t data);

    // Community with own buss
    // I think this is simple, because this is just community with it's own bus
    uint8_t ppuRead(uint16_t addr, bool readOnly = false);
    void ppuWrite(uint16_t addr, uint8_t data);

public:
    olc::Sprite& getPattern(int which, int nPalette);

private:
    Palette palette;
    Pattern pattern;

    // 0x3f00 - 0x3f1d
    // 0x3f00 single entry for the background color?? 8 bit value? 64 color.
    // why the video palette is not continued?
    uint8_t tblPalette[32]{};


    // Yes, we can community with cart.
    std::shared_ptr<Cartridge> cart;

private:
    // | olc::Sprite - An image represented by a 2D array of olc::Pixel               |
    olc::Sprite sprScreen = olc::Sprite(width, height);
    olc::Sprite sprNameTable[2] = {olc::Sprite(width, height), olc::Sprite(width, height)};

public:
    // Helper method
    olc::Pixel getColorInPalette(int palette, int index);
    // Debugging Utilities
    olc::Sprite& GetScreen();
    olc::Sprite& GetNameTable(uint8_t which);

    bool frame_complete = false;

private:
    int16_t scanline = 0;
    uint16_t cycle = 0;
    olc::Pixel white = olc::Pixel(236, 238, 236);
    olc::Pixel black = olc::Pixel(0, 0, 0);

public:
    // Interface
    void connectCartridge(const std::shared_ptr<Cartridge>& cartridge);
    // So ppu has it's own clock
    // But what a ppu clock do?
    void clock();


private:
    uint16_t ppu_address = 0x0000;
    // I don't know what's the nmi
    bool nmi = false;
    void forward_ppu_address();
};


#endif //NES_PPU_H
