//
// Created by huhua on 2021/6/21.
//

#include <cassert>
#include "bridge.h"
#include <iostream>

static const int OP_CONTROL = 0x0;
static const int OP_MASK = 0x1;
static const int OP_STATUS = 0x2;
static const int OP_OAM_ADDR = 0x3;
static const int OP_OAM_DATA = 0x4;
static const int OP_SCROLL = 0x5;
static const int OP_ADDR = 0x6;
static const int OP_DATA = 0x7;

uint8_t Bridge::read(uint16_t addr, bool readOnly) {
    assert(addr <= 7);

    uint8_t data = 0x00;
    switch (addr) {
        case OP_CONTROL:
            std::cout << "why you read from control" << std::endl;
            break;
        case OP_MASK:    // Mask
            std::cout << "why you read from mask" << std::endl;
            break;
        case OP_STATUS:    // Status
            // why you can do this??
            // status.vertical_blank = 1;
            data = (status.reg & 0xE0) | (ppu_data_buffer & 0x1f);
            status.vertical_blank = 0;
            address_latch = 0;
            break;
        case OP_OAM_ADDR:    // ignore for now NOLINT(bugprone-branch-clone)
            break;
        case OP_OAM_DATA:    // ignore for now
            break;
        case OP_SCROLL:    // Scroll
            std::cout << "why you read from scroll" << std::endl;
            break;
        case OP_ADDR:    // PPU Address
            std::cout << "why you read ppu address?" << std::endl;
            break;
        case OP_DATA:    // PPU Data
            // delayed one cycle, I dont know why
            data = ppu_data_buffer;
            ppu_data_buffer = ppuRead(ppu_address);
            // palette has no delay
            if (ppu_address >= palette.addr_min) data = ppu_data_buffer;
            forward_ppu_address();
            break;
    }
    return data;
    return 0;
}

void Bridge::write(uint16_t addr, uint8_t data) {

}