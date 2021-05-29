//
// Created by huhua on 2021/5/22.
//

#include "bus.h"

Bus::Bus() {
    for (auto &i: ram) {
        i = 0x00;
    }
    cpu.connectBus(this);
}

Bus::~Bus() {

}

void Bus::write(uint16_t addr, uint8_t data) {
    // eat error
    if (addr <= 0xffff) {
        ram[addr] = data;
    }
}

uint8_t Bus::read(uint16_t addr, bool readOnly) {
    // eat error
    if (addr <= 0xffff) {
        return ram[addr];
    }
    return 0;
}
