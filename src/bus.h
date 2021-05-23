//
// Created by huhua on 2021/5/22.
//

#ifndef NES_BUS_H
#define NES_BUS_H

#include <cstdint>
#include <array>
#include "cpu.h"

class Bus {
private:
    // 64kb
    std::array<uint8_t, 0x10000> ram;

public:
    Bus();
    ~Bus();

public:
    // Devices
    CPU cpu;

    uint8_t read(uint16_t addr, bool readOnly = false);
    void write(uint16_t addr, uint8_t data);
};


#endif //NES_BUS_H
