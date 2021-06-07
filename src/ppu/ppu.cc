//
// Created by hwf on 2021/6/4.
//

#include "ppu.h"

uint32_t PPU::TILE_SIZE = 16;
uint32_t PPU::PIXEL_SIZE_PER_TILE = 8;
uint32_t PPU::PIXEL_SIZE = TILE_SIZE * PIXEL_SIZE_PER_TILE;
uint32_t PPU::BYTE_SIZE_PRE_TILE = 8 * 2;

uint8_t PPU::cpuRead(uint16_t addr, bool readOnly) {
    uint8_t rst = 0x00;
    switch (addr) {
        case 0x0000:    // Control
            rst = control.reg;
            break;
        case 0x0001:    // Mask
            rst = mask.reg;
            break;
        case 0x0002:    // Status
            break;
        case 0x0003:    // OAM Address
            break;
        case 0x0004:    // OAM Data
            break;
        case 0x0005:    // Scroll
            break;
        case 0x0006:    // PPU Address
            break;
        case 0x0007:    // PPU Data
            break;
    }
    return rst;
}

/**
 * Cpu Write is strange. because it's seems like that cpu write
 * to some it's self. address.
 * @param addr
 * @param data
 */
void PPU::cpuWrite(uint16_t addr, uint8_t data) {
    switch (addr) {
        case 0x0000:    // Control
            control.reg = data;
            break;
        case 0x0001:    // Mask
            mask.reg = data;
            break;
        case 0x0002:    // Status
            break;
        case 0x0003:    // OAM Address
            break;
        case 0x0004:    // OAM Data
            break;
        case 0x0005:    // Scroll
            break;
        case 0x0006:    // PPU Address
            break;
        case 0x0007:    // PPU Data
            break;
    }
}

uint8_t PPU::ppuRead(uint16_t addr, bool readOnly) {
    uint8_t data = 0x00;
    if (cart->ppuRead(addr, data)) {

    } else {
        addr &= 0x3fff;
        if (addr >= palette.addr_min && addr <= palette.addr_max) {
            data = palette.read(addr);
        }
    }
    return data;
}

void PPU::ppuWrite(uint16_t addr, uint8_t data) {
    if (cart->ppuWrite(addr, data)) {

    } else {
        addr &= 0x3fff;
        if (addr >= palette.addr_min && addr <= palette.addr_max) {
            data = palette.write(addr, data);
        }
    }
}

void PPU::connectCartridge(const std::shared_ptr<Cartridge> &cartridge) {
    this->cart = cartridge;
}

olc::Sprite &PPU::GetScreen() {
    return sprScreen;
}

olc::Sprite &PPU::GetNameTable(uint8_t which) {
    return sprNameTable[which];
}

void PPU::clock() {
    sprScreen.SetPixel(cycle - 1, scanline, rand() % 2? black : white);

    cycle++;

    // why 341?
    if (cycle >= 341) {
        cycle = 0;
        scanline++;

        if (scanline >= 261) {
            // back to begin.
            scanline = -1;
            frame_complete = true;
        }
    }
}

PPU::PPU() {
}

olc::Sprite &PPU::GetPatternTable(uint8_t which, uint8_t whichPalette) {
    // How to loop set the pixel?

    // a pattern table has 128 * 128 pixels
    for (int tileY = 0; tileY < TILE_SIZE; tileY++) {
        for (int tileX = 0; tileX < TILE_SIZE; tileX++) {
            // Ok One tile
            uint32_t bytesPerTable = 4 * 1024;
            uint32_t bytesBase = bytesPerTable * which + (tileY * TILE_SIZE + tileX)  * BYTE_SIZE_PRE_TILE;

            // Now we have located the tile.
            // A tile is 8 * 8 pixel. which 16 * 8 bytes.

            // ok, now calculate the tile
            for (int y = 0; y < 8; y++) {
                for (int x = 0; x < 8; x++) {

                }
            }
        }
    }
    return sprPatternTable[which];
}

olc::Pixel PPU::getColorInPalette(int which_palette, int index) {
    return palette.getColor(which_palette, index);
}
