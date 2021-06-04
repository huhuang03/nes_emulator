//
// Created by hwf on 2021/6/4.
//

#ifndef NES_CARTRIDGE_H
#define NES_CARTRIDGE_H


#include <cstdint>
#include <vector>
#include <string>

#include "mapper_000.h"

// prg is 16kb units
// chr is 8kb units
class Cartridge {
private:
    static uint32_t UNIT_PRG;
    static uint32_t UNIT_CHR;

private:
    std::vector<uint8_t> vPRGMemory;
    std::vector<uint8_t> vCHRMemory;

    uint8_t nMapperID = 0;
    uint8_t nPRGBanks = 0;
    uint8_t nCHRBanks = 0;

    std::shared_ptr<Mapper> pMapper;

public:
    explicit Cartridge(const std::string& sFileName);

public:
    bool cpuRead(uint16_t addr, uint8_t& data);
    bool cpuWrite(uint16_t addr, uint8_t data);

    // Community with own buss
    bool ppuRead(uint16_t addr, uint8_t& data);
    bool ppuWrite(uint16_t addr, uint8_t data);
};


#endif //NES_CARTRIDGE_H
