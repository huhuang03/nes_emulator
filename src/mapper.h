//
// Created by hwf on 2021/6/4.
//

#ifndef NES_MAPPER_H
#define NES_MAPPER_H

#include <cstdint>

/**
 * Yes, a mapper can take care of bus read and write cpu, ppu.
 */
class Mapper {
protected:
    uint16_t addr_cpu_min = 0x8000;
    uint16_t addr_cpu_max = 0xFFFF;
    uint16_t addr_ppu_min = 0x0000;
    uint16_t addr_ppu_max = 0x1fff;

public:
    Mapper(uint8_t prgBanks, uint8_t chrBanks);

public:
    virtual bool cpuMapRead(uint16_t addr, uint32_t &mapped_addr) = 0;
    virtual bool cpuMapWrite(uint16_t addr, uint32_t &mapped_addr) = 0;
    virtual bool ppuMapRead(uint16_t addr, uint32_t &mapped_addr) = 0;
    virtual bool ppuMapWrite(uint16_t addr, uint32_t &mapped_addr) = 0;

protected:
    uint8_t nPRGBanks = 0;
    uint8_t nCHRBanks = 0;
};


#endif //NES_MAPPER_H
