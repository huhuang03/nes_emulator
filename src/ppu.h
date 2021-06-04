//
// Created by hwf on 2021/6/4.
//

#ifndef NES_PPU_H
#define NES_PPU_H


#include <cstdint>
#include <memory>
#include "cartridge.h"

// 2C02
class PPU {
public:
    // Community with cpu
    uint8_t cpuRead(uint16_t addr, bool readOnly = false);
    void cpuWrite(uint16_t addr, uint8_t data);

    // Community with own buss
    uint8_t ppuRead(uint16_t addr, bool readOnly = false);
    void ppuWrite(uint16_t addr, uint8_t data);

private:
    // Devices connected to it's bus
    uint8_t tblName[2][1024];   // 2kb
    uint8_t tblPalette[32];
    uint8_t tblPattern[2][4096];

    // Yes, we can community with cart.
    std::shared_ptr<Cartridge> cart;

public:
    // Interface
    void connectCartridge(const std::shared_ptr<Cartridge>& cartridge);
    void clock();
};


#endif //NES_PPU_H
