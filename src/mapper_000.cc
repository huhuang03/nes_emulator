//
// Created by hwf on 2021/6/4.
//

#include <iostream>
#include "mapper_000.h"

namespace th {
    bool Mapper000::cpuMapRead(uint16_t addr, uint32_t &mapped_addr) {
        // so you can take care of 0x8000 and 0xFFFF
        if (addr >= addr_cpu_min && addr <= addr_cpu_max) {
            mapped_addr = addr & (nPRGBanks > 1 ? 0x7FFF : 0x3FFF);
            return true;
        }
        return false;
    }

    bool Mapper000::cpuMapWrite(uint16_t addr, uint32_t &mapped_addr) {
        if (addr >= addr_cpu_min && addr <= addr_cpu_max) {
            mapped_addr = addr & (nPRGBanks > 1 ? 0x7FFF : 0x3FFF);
            return true;
        }
        return false;
    }

    bool Mapper000::ppuMapRead(uint16_t addr, uint32_t &mapped_addr) {
        if (addr >= addr_ppu_min && addr <= addr_ppu_max) {
            mapped_addr = addr;
            return true;
        }
        return false;
    }

    bool Mapper000::ppuMapWrite(uint16_t addr, uint32_t &mapped_addr) {
        return false;
    }

    Mapper000::Mapper000(uint8_t prgBanks, uint8_t chrBanks) : Mapper(prgBanks, chrBanks) {
//    std::cout << "Mapper000 " << std::to_string(prgBanks) << std::to_string(chrBanks) << std::endl;
    }
}