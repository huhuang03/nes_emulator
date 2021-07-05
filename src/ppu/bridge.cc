//
// Created by huhua on 2021/6/21.
//

#include <cassert>
#include "bridge.h"
#include <iostream>
#include "./ppu.h"

namespace th {
    static const int OP_CONTROL = 0x0;
    static const int OP_MASK = 0x1;
    static const int OP_STATUS = 0x2;
    static const int OP_OAM_ADDR = 0x3;
    static const int OP_OAM_DATA = 0x4;
    static const int OP_SCROLL = 0x5;
    static const int OP_ADDR = 0x6;
    static const int OP_DATA = 0x7;

    /**
     * @param readOnly If need read the status, without effect other, like assembly. we need readOnly
     */
    uint8_t Bridge::read(uint16_t addr, bool readOnly) {
        assert(addr <= 7);

        uint8_t data = 0x00;

        if (readOnly) {
            switch (addr) {
                case OP_CONTROL:
                    data = ppu->control.reg;
                    ppu->tram_addr.nametable_x = ppu->control.nametable_x;
                    ppu->tram_addr.nametable_y = ppu->control.nametable_y;
                    break;
                case OP_MASK:
                    data = ppu->mask.reg;
                    break;
                case OP_STATUS:
                    data = ppu->status.reg;
                    break;
                default:
                    break;
            }
            return data;
        }


        switch (addr) {
            case OP_CONTROL:
//            std::cout << "why you read from control" << std::endl;
                break;
            case OP_MASK:    // Mask
//            std::cout << "why you read from mask" << std::endl;
                break;
            case OP_STATUS:    // Status
                // why you can do this??
                // not work at all.
                // why I dont' set this, also passed?
                // ppu->status.vertical_blank = 1;
                // 0xcc??
                data = (ppu->status.reg & 0xE0) | (ppu_data_buffer & 0x1f);
                ppu->status.vertical_blank = 0;
                address_latch = 0;
                break;
            case OP_OAM_ADDR:    // ignore for now NOLINT(bugprone-branch-clone)
                break;
            case OP_OAM_DATA:    // ignore for now
                break;
            case OP_SCROLL:    // Scroll
                if (address_latch == 0) {
                    ppu->fine_x = data & 0x07;
                    ppu->tram_addr.coarse_x = data >> 3;
                    address_latch = 1;
                } else {
                    ppu->tram_addr.fine_y = data & 0x07;
                    ppu->tram_addr.coarse_y = data >> 3;
                    address_latch = 0;
                }
                break;
            case OP_ADDR:    // PPU Address
                if (address_latch == 0) {
                    // First set the high byte(6)
                    // the rest or is duoyu, but I set it for now
                    ppu->tram_addr.reg = data << 8 | (ppu->tram_addr.reg & 0xff);
                    address_latch = 1;
                } else {
                    // Then set the low byte()
                    ppu->tram_addr.reg = (ppu->tram_addr.reg & 0xff00) | data;
                    ppu->vram_addr = ppu->tram_addr;
                    address_latch = 0;
                }
                break;
            case OP_DATA:    // PPU Data
                // delayed one cycle, I dont know why
                data = ppu_data_buffer;
                ppu_data_buffer = ppu->ppuRead(ppu->vram_addr.reg);
                // palette has no delay
                if (ppu->vram_addr.reg >= ppu->palette.addr_min) data = ppu_data_buffer;
                forward_ppu_address();
                break;
        }
        return data;
    }

    void Bridge::write(uint16_t addr, uint8_t data) {
        switch (addr) {
            case OP_CONTROL:
                ppu->control.reg = data;
                break;
            case OP_MASK:
                ppu->mask.reg = data;
                break;
            case OP_STATUS:
                // you can't write status?
                std::cout << "why you can write to status? " << std::endl;
                // throw std::runtime_error("Actually, you can't write to status.");
                break;
            case OP_OAM_ADDR:
                break;
            case OP_OAM_DATA:
                break;
            case OP_SCROLL:
                break;
            case OP_ADDR:
                if (address_latch == 0) {
                    // First set the high byte(6)
                    // the rest or is duoyu, but I set it for now
                    ppu->vram_addr.reg = (data & 0x003F) << 8 | (ppu->vram_addr.reg & 0xff);
                    address_latch = 1;
                } else {
                    // Then set the low byte()
                    ppu->vram_addr.reg = (ppu->vram_addr.reg & 0xff00) | data;
                    address_latch = 0;
                }
                break;
            case OP_DATA:
                ppu->ppuWrite(ppu->vram_addr.reg, data);
                forward_ppu_address();
                break;
        }
    }

    void Bridge::setPPU(PPU *ppu1) {
        this->ppu = ppu1;
    }

    void Bridge::forward_ppu_address() {
        // I don't know why 32 now.
        ppu->vram_addr.reg += (ppu->control.increment_mode ? 32 : 1);
    }

}