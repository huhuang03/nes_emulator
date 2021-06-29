//
// Created by huhua on 2021/6/28.
//

#ifndef NES_EMPTY_CARTRIDGE_H
#define NES_EMPTY_CARTRIDGE_H

#include "./cartridge.h"

namespace th {
    class EmptyCartridge : public Cartridge {
    public:
        EmptyCartridge() : Cartridge("") {};
    private:
        uint8_t ram[0xFFFF] = {0};

    public:
        bool cpuRead(uint16_t addr, uint8_t &data) override;

        bool cpuWrite(uint16_t addr, uint8_t data) override;

    };
}
#endif //NES_EMPTY_CARTRIDGE_H
