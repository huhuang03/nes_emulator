//
// Created by huhua on 2021/6/28.
//

#include "empty_cartridge.h"

bool EmptyCartridge::cpuRead(uint16_t addr, uint8_t &data) {
    data = this->ram[addr];
    return true;
}

bool EmptyCartridge::cpuWrite(uint16_t addr, uint8_t data) {
    this->ram[addr] = data;
    return true;
}
