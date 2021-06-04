//
// Created by hwf on 2021/6/4.
//

#include "ppu.h"

uint8_t PPU::cpuRead(uint16_t addr, bool readOnly) {
    uint8_t rst = 0x00;
    switch (addr) {
        case 0x0000:    // Control
            break;
        case 0x0001:    // Mask
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

void PPU::cpuWrite(uint16_t addr, uint8_t data) {
    switch (addr) {
        case 0x0000:    // Control
            break;
        case 0x0001:    // Mask
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
    }
    return data;
}

void PPU::ppuWrite(uint16_t addr, uint8_t data) {
    if (cart->ppuWrite(addr, data)) {

    } else {
        addr &= 0x3fff;
    }
}

void PPU::connectCartridge(const std::shared_ptr<Cartridge> &cartridge) {
    this->cart = cartridge;
}
