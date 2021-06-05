//
// Created by hwf on 2021/6/4.
//

#ifndef NES_PPU_H
#define NES_PPU_H


#include <cstdint>
#include <memory>
#include "cartridge.h"

#include "./olcPixelGameEngine.h"

// model: 2C02
/**
 * We use pixel game engine to draw the game.
 *
 * 0x0000 - 0x1FFF Pattern Memory "CHR ROM"
 * 0x2000 - 0x3EFF Name Table Memory
 * 0x3F00 - 0x3FFF Palette Memory
 */
class PPU {
public:
    PPU();
    // Community with cpu
    // Who and how call you?
    uint8_t cpuRead(uint16_t addr, bool readOnly = false);
    void cpuWrite(uint16_t addr, uint8_t data);

    // Community with own buss
    // I think this is simple, because this is just community with it's own bus
    uint8_t ppuRead(uint16_t addr, bool readOnly = false);
    void ppuWrite(uint16_t addr, uint8_t data);

private:
    // Devices connected to it's bus
    uint8_t tblName[2][1024];   // 2kb
    uint8_t tblPalette[32];
    uint8_t tblPattern[2][4096];

    // Yes, we can community with cart.
    std::shared_ptr<Cartridge> cart;

private:
    // pal look like color table.
    // | olc::Pixel - Represents a 32-Bit RGBA colour                                 |
    olc::Pixel palScreen[0x40];
    // | olc::Sprite - An image represented by a 2D array of olc::Pixel               |
    olc::Sprite sprScreen = olc::Sprite(256, 240);
    olc::Sprite sprNameTable[2] = {olc::Sprite(256, 240), olc::Sprite(256, 240)};
    olc::Sprite sprPatternTable[2] = {olc::Sprite(128, 128), olc::Sprite(128, 128)};

public:
    // Debugging Utilities
    olc::Sprite& GetScreen();
    olc::Sprite& GetNameTable(uint8_t which);
    olc::Sprite& GetPatternTable(uint8_t which);
    bool frame_complete = false;

private:
    int16_t scanline = 0;
    uint16_t cycle = 0;

public:
    // Interface
    void connectCartridge(const std::shared_ptr<Cartridge>& cartridge);
    // So ppu has it's own clock
    // But what a ppu clock do?
    void clock();
};


#endif //NES_PPU_H
