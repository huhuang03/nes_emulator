//
// Created by hwf on 2021/6/4.
//

#ifndef NES_PPU_H
#define NES_PPU_H


#include <cstdint>
#include <memory>
#include "th_nes/cartridge.h"

#include "../olcPixelGameEngine.h"
#include "palette.h"
#include "pattern.h"
#include "./bridge.h"
#include "./name_tables.h"

// model: 2C02
/**
 * We use pixel game engine to drawNo the game.
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
namespace th {
    class PPU {
        // Now, let's remove some magic
    private:
        const uint16_t scan_width = 341;
        const uint16_t scan_height = 261;
        const uint16_t width = 256;
        const uint16_t validate_scan_lines = 240;

        friend class Bridge;

        friend class NameTables;

        friend class UIPPU;

        friend class Pattern;

    public:
        // I don't know what's the nmi
        bool nmi = false;

    public:
        PPU();

        void reset();

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
        olc::Sprite &getPattern(int which, int nPalette);

        Palette palette;

    private:
        Pattern pattern;
        Bridge bridge;
        NameTables nameTables;

        // Yes, we can community with cart.
        std::shared_ptr<Cartridge> cart;

    private:
        // | olc::Sprite - An image represented by a 2D array of olc::Pixel               |
        // 这里居然回收错误？
        olc::Sprite sprScreen = olc::Sprite(width, validate_scan_lines);

    public:
        // Helper method
        olc::Pixel getColorInPalette(int palette, int index);

        // Debugging Utilities
        olc::Sprite &GetScreen();

        bool frame_complete = false;

    private:
        int16_t scanline = 0;
        uint16_t cycle = 0;
        olc::Pixel white = olc::Pixel(236, 238, 236);
        olc::Pixel black = olc::Pixel(0, 0, 0);

    public:
        // Interface
        void connectCartridge(const std::shared_ptr<Cartridge> &cartridge);

        // So ppu has it's own clock
        // But what a ppu clock do?
        void clock();

    private:
        // The state reg.
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
                uint8_t unused: 5;
                uint8_t sprite_overflow: 1;
                uint8_t sprite_zero_hit: 1;
                // In the video, It seems says that when scan the out of the height, means vertical_blank
                uint8_t vertical_blank: 1;
            };
            uint8_t reg;
        } status;

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


    private:
    };
}
#endif //NES_PPU_H
