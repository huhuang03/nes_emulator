//
// Created by huhua on 2021/5/22.
//

#include "th_nes/bus.h"
#include "util.h"

/**
 * Ok, we can hardly create an empty cartridge. So we check the cart.
 */

namespace th {
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
        } else if (addr >= 0x4016 && addr <= 0x4017)
        {
            controller_state[addr & 0x0001] = controller[addr & 0x0001];
        }
    }

    uint8_t Bus::read(uint16_t addr, bool readOnly) {
        uint8_t data = 0;
        if (cart != nullptr && cart->cpuRead(addr, data)) {

        } else if (addr <= 0x1fff) {   // ram
            data = cpuRam[addr & 0x07ff];
        } else if (addr >= 0x2000 && addr <= 0x3fff) {  // ppu
            data = ppu.cpuRead(addr & 0x0007, readOnly);
        } else if (addr >= 0x4016 && addr <= 0x4017)
        {
            data = (controller_state[addr & 0x0001] & 0x80) > 0;
            controller_state[addr & 0x0001] <<= 1;
        }
        return data;
    }

    void Bus::reset() {
        if (cart != nullptr) {
            cart->reset();
        }
        cpu.reset();
        ppu.reset();
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

        if (ppu.nmi) {
            // enter interpreter?
            ppu.nmi = false;
            cpu.nmi();
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

}