//
// Created by hwf on 2021/6/4.
//

#include "ppu.h"
#include "../util.h"

namespace th {

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

        } else if (addr >= nameTables.addr_min && addr <= nameTables.addr_max) {
            data = nameTables.read(addr);
        } else if (addr >= palette.addr_min && addr <= palette.addr_max) {
            data = palette.read(addr, mask.grayscale);
        }
        // should throw here?
        return data;
    }

    static int count = 0;

    void PPU::ppuWrite(uint16_t addr, uint8_t data) {
        // why mirror??
        if (addr >= nameTables.addr_min && addr <= nameTables.addr_max) {
            // The data is not right.
            // but the data is not right
            if (data == 0x20) {
                std::cout << "write 0x20 to nameTables: " << hex(addr, 4) << ", val: " << hex(data, 1) << std::endl;
            }
            if (addr == nameTables.addr_min) {
                // pc is 49207 0xc037
//                std::cout << "write to nameTables: " << hex(addr, 4) << ", val: " << hex(data, 1) << std::endl;
                // fuck, I want know the cpu ip
//            std::cout << "pc: " <<  << std::endl;
            }
            nameTables.write(addr, data);
        } else if (addr >= palette.mirror_min && addr <= palette.mirror_max) {
            palette.write(addr, data);
        }
        // should throw here?
    }

    void PPU::connectCartridge(const std::shared_ptr<Cartridge> &cartridge) {
        this->cart = cartridge;
    }

    olc::Sprite &PPU::GetScreen() {
        return sprScreen;
    }

    void PPU::clock() {
        // leave vertical black
        if (scanline == -1 && cycle == 1) {
            status.vertical_blank = 0;
        }


        if (scanline == validate_scan_lines + 1 && cycle == 1) {
            // enter vertical_blank
            status.vertical_blank = 1;
            if (control.enable_nmi) {
                nmi = true;
            }
        }


//        sprScreen.SetPixel(cycle - 1, scanline, rand() % 2 ? black : white);

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
        this->bridge.setPPU(this);
        this->pattern.setPPU(this);
        this->palette.setPPU(this);
        this->nameTables.setPPU(this);
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

    void PPU::reset() {
        this->status.reg = 0;
        this->control.reg = 0;
        this->mask.reg = 0;
    }

}