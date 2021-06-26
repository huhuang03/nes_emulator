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

void Bus::write(uint16_t addr, uint8_t data) {
    // why cartridge write all the addr??
    if (cart->cpuWrite(addr, data)) {

    } else if (addr <= 0x1fff) {    // ram
        cpuRam[addr & 0x07ff] = data;
    } else if (addr >= 0x2000 && addr <= 0x3fff) {  // ppu
        ppu.cpuWrite(addr & 0x0007, data);
    }
}

uint8_t Bus::read(uint16_t addr, bool readOnly) {
    uint8_t data = 0;
    if (cart->cpuRead(addr, data)) {

    } else if (addr <= 0x1fff) {   // ram
        data = cpuRam[addr & 0x07ff];
    } else if (addr >= 0x2000 && addr <= 0x3fff) {  // ppu
        data = ppu.cpuRead(addr & 0x0007, readOnly);
    }
    return data;
}

void Bus::reset() {
    cart->reset();
    cpu.reset();
    nSystemClockCounter = 0;
}

/**
 * plugin cartridge and connect ppu
 */
void Bus::insertCartridge(const std::shared_ptr<Cartridge> &cartridge) {
    this->cart = cartridge;
    ppu.connectCartridge(cartridge);
}

void Bus::clock() {
    ppu.clock();

    if (nSystemClockCounter % 3 == 0) {
        // cpu is slower than ppu.
        cpu.clock();
    }

    nSystemClockCounter++;
}

// bn
void Bus::clockCpu() {
    while (!cpu.complete()) {
        clock();
    }

    // go next
    while (cpu.complete()) {
        clock();
    }
}
