//
// Created by hwf on 2021/6/4.
//

#include "ppu.h"

uint32_t PPU::TILE_SIZE = 16;
uint32_t PPU::PIXEL_SIZE_PER_TILE = 8;
uint32_t PPU::PIXEL_SIZE = TILE_SIZE * PIXEL_SIZE_PER_TILE;
uint32_t PPU::BYTE_SIZE_PRE_TILE = 8 * 2;

uint8_t PPU::cpuRead(uint16_t addr, bool readOnly) {
    uint8_t data = 0x00;
    switch (addr) {
        case 0x0000:    // Control
            data = control.reg;
            break;
        case 0x0001:    // Mask
            data = mask.reg;
            break;
        case 0x0002:    // Status
            // still some strange thing
            // yes, it's amazing.
            // call read 2002 -> bus.read(2002) -> ppu.cpuRead(0x2)
            status.vertical_blank = 1;
            data = (status.reg & 0xE0) | (ppu_data_buffer & 0x1f);
            status.vertical_blank = 0;
            address_latch = 0;
            break;
        case 0x0003:    // OAM Address
            break;
        case 0x0004:    // OAM Data
            break;
        case 0x0005:    // Scroll
            break;
        case 0x0006:    // PPU Address
//            throw std::runtime_error("Should not happen, read the ppu address");
            std::cout << "why you read ppu address?" << std::endl;
            break;
        case 0x0007:    // PPU Data
            // delayed one cycle, I dont know why
            data = ppu_data_buffer;
            ppu_data_buffer = ppuRead(ppu_address);
            // palette has no delay
            if (ppu_address >= palette.addr_min) data = ppu_data_buffer;
            forward_ppu_address();
            break;
    }
    return data;
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
            if (address_latch == 0) {
                // First set the high byte(6)
                ppu_address = (data & 0x003F) << 8;
                address_latch = 1;
            } else {
                // Then se the low byte()
                ppu_address = (ppu_address & 0xff00) | data;
                address_latch = 0;
            }
            break;
        case 0x0007:    // PPU Data
            ppuWrite(ppu_address, data);
            forward_ppu_address();
            break;
    }
}

uint8_t PPU::ppuRead(uint16_t addr, bool readOnly) {
    uint8_t data = 0x00;
    if (cart->ppuRead(addr, data)) {

    } else {
        addr &= 0x3fff;
        if (addr >= palette.addr_min && addr <= palette.addr_max) {
            data = palette.read(addr, mask.grayscale);
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
    // we are no output of the height
    // why judge the cycle, I don't now for now.
    // and what is the cycle, I don't know either.
    if (scanline == height + 1 && cycle == 1) {
        // what is the vertical_blank
        status.vertical_blank = 1;
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

void PPU::forward_ppu_address() {
    // I don't know why 32 now.
    ppu_address += (control.increment_mode ? 32: 1);
}

olc::Sprite &PPU::getPattern(int which, int nPalette) {
    return this->pattern.getSprite(which, nPalette);
}
