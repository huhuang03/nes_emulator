//
// Created by hwf on 2021/6/4.
//

#include "ppu.h"


uint8_t PPU::cpuRead(uint16_t addr, bool readOnly) {
    return this->bridge.read(addr, readOnly);
}

/**
 * Cpu Write is strange. because it's seems like that cpu write
 * to some it's self. address.
 * @param addr
 * @param data
 */
void PPU::cpuWrite(uint16_t addr, uint8_t data) {
    this->bridge.write(addr, data);
}

uint8_t PPU::ppuRead(uint16_t addr, bool readOnly) {
    uint8_t data = 0x00;
    if (cart->ppuRead(addr, data)) {

    } else {
        addr &= 0x3fff;
        if (addr >= palette.addr_min && addr <= palette.addr_max) {
            data = palette.read(addr, this->bridge.mask.grayscale);
        }
    }
    return data;
}

void PPU::ppuWrite(uint16_t addr, uint8_t data) {
    // why mirror??
    addr &= 0x3fff;
    if (cart->ppuWrite(addr, data)) {

    } else if (addr >= palette.mirror_min && addr <= palette.mirror_max) {
        palette.write(addr, data);
    } else {
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
    if (scanline == -1 && cycle == 1) {
        this->bridge.status.vertical_blank = 0;
    }

    // we are no output of the height
    // why judge the cycle, I don't now for now.
    // and what is the cycle, I don't know either.
    if (scanline == height + 1 && cycle == 1) {
        // what is the vertical_blank
        this->bridge.status.vertical_blank = 1;
        if (this->bridge.control.enable_nmi) {
            nmi = true;
        }
    }

    sprScreen.SetPixel(cycle - 1, scanline, rand() % 2? black : white);

    cycle++;

    // why 341?
    if (cycle >= this->scan_width) {
        cycle = 0;
        scanline++;

        if (scanline >= scan_height) {
            // back to begin.
            scanline = -1;
            frame_complete = true;
        }
    }
}

PPU::PPU() {
    this->pattern.setPPU(this);
    this->palette.setPPU(this);
}

olc::Pixel PPU::getColorInPalette(int which_palette, int index) {
    if (index < 0 || index > 3) {
        throw std::runtime_error("index range is [" + std::to_string(0) + " - " + std::to_string(3) + "]");
    }
//    if (index > 0) {
//        std::cout << "index: " << std::to_string(index) << std::endl;
//    }
    return palette.getColor(ppuRead(palette.addr_min + which_palette * 4 + index));
}

olc::Sprite &PPU::getPattern(int which, int nPalette) {
    return this->pattern.getSprite(which, nPalette);
}
