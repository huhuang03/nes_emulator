//
// Created by huhua on 2021/5/22.
//

#include "bus.h"

Bus::Bus() {
    for (auto &i: cpuRam) {
        i = 0x00;
    }
    cpu.connectBus(this);
}

Bus::~Bus() {

}

void Bus::cpuWrite(uint16_t addr, uint8_t data) {
    // why cartridge write all the addr??
    if (cart->cpuWrite(addr, data)) {

    } else if (addr <= 0x1fff) {    // ram
        cpuRam[addr & 0x07ff] = data;
    } else if (addr >= 0x2000 && addr <= 0x3fff) {  // ppu
        ppu.cpuWrite(addr & 0x0007, data);
    }
}

uint8_t Bus::cpuRead(uint16_t addr, bool readOnly) {
    uint8_t data;
    if (cart->cpuRead(addr, data)) {

    } else if (addr <= 0x1fff) {   // ram
        data = cpuRam[addr & 0x07ff];
    } else if (addr >= 0x2000 && addr <= 0x3fff) {  // ppu
        data = ppu.cpuRead(addr & 0x0007, readOnly);
    }
    return data;
}

void Bus::reset() {
    nSystemClockCounter = 0;
    cpu.clock();
}

void Bus::insertCartridge(const std::shared_ptr<Cartridge> &cartridge) {
    this->cart = cartridge;
    ppu.connectCartridge(cartridge);
}
