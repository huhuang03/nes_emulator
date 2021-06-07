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
    /**
     * How many TILES_NUM * TILES_NUM in one pattern table.
     */
    static uint32_t TILE_SIZE;

    static uint32_t PIXEL_SIZE_PER_TILE;

    static uint32_t PIXEL_SIZE;
    
    static uint32_t BYTE_SIZE_PRE_TILE;

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

private:
    Palette palette;

    // A Tile's value is combined by two 8 x 8 bit table. And those two table is follow in physic
    uint8_t tblPattern[2][4096];    // 8kb

    // Devices connected to it's bus
    uint8_t tblName[2][1024];   // 2kb

    // 0x3f00 - 0x3f1d
    // 0x3f00 single entry for the background color?? 8 bit value? 64 color.
    // why the video palette is not continued?
    uint8_t tblPalette[32];


    // Yes, we can community with cart.
    std::shared_ptr<Cartridge> cart;

private:
    // | olc::Sprite - An image represented by a 2D array of olc::Pixel               |
    olc::Sprite sprScreen = olc::Sprite(256, 240);
    olc::Sprite sprNameTable[2] = {olc::Sprite(256, 240), olc::Sprite(256, 240)};
    olc::Sprite sprPatternTable[2] = {olc::Sprite(PIXEL_SIZE, PIXEL_SIZE)
                                      , olc::Sprite(PIXEL_SIZE, PIXEL_SIZE)};

public:
    // Helper method
    olc::Pixel getColorInPalette(int palette, int index);
    // Debugging Utilities
    olc::Sprite& GetScreen();
    olc::Sprite& GetNameTable(uint8_t which);

    /**
     * We need calculate the Sprite in time for which pattern and which pallette
     */
    olc::Sprite& GetPatternTable(uint8_t which, uint8_t whichPalette);
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
    union {
        struct {
            uint8_t unused: 5;
            uint8_t sprite_overflow: 1;
            uint8_t sprite_zero_hit: 1;
            uint8_t vertical_blank: 1;
        };
        uint8_t reg;
    } status;

    union {
        struct {
            uint8_t grayscale: 1;
            uint8_t render_background_left: 1;
            uint8_t render_sprites_left: 1;
            uint8_t render_background: 1;
            uint8_t render_sprites: 1;
            uint8_t enhance_red: 1;
            uint8_t enhance_green: 1;
            uint8_t enhance_blue: 1;
        };
        uint8_t reg;
    } mask;

    union {
        struct {
            uint8_t nametable_x: 1;
            uint8_t nametable_y: 1;
            uint8_t increment_mode: 1;
            uint8_t pattern_sprite: 1;
            uint8_t pattern_background: 1;
            uint8_t sprite_size: 1;
            uint8_t slave_mode: 1;
            uint8_t enable_nmi: 1;
        };
        uint8_t reg;
    } control;
};


#endif //NES_PPU_H
