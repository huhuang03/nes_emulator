//
// Created by huhua on 2021/5/22.
//

#ifndef NES_BUS_H
#define NES_BUS_H

#include <cstdint>
#include <array>
#include <memory>
#include "cpu.h"
#include "ppu.h"
#include "cartridge.h"

class Bus {
private:
    // 2kb
    std::array<uint8_t, 0x800> cpuRam;
    PPU ppu;
    std::shared_ptr<Cartridge> cart;

    // A count of how many clocks have passed.
    uint32_t nSystemClockCounter = 0;

public:
    Bus();
    ~Bus();

public:
    // Devices
    CPU cpu;

    uint8_t cpuRead(uint16_t addr, bool readOnly = false);
    void cpuWrite(uint16_t addr, uint8_t data);

public: // System Interface
    void insertCartridge(const std::shared_ptr<Cartridge>& cartridge);
    void reset();
    void clock();
};


#endif //NES_BUS_H
