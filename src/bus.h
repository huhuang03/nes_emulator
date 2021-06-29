//
// Created by huhua on 2021/5/22.
//

#ifndef NES_BUS_H
#define NES_BUS_H

#include <cstdint>
#include <array>
#include <memory>
#include "cpu.h"
#include "ppu/ppu.h"
#include "cartridge.h"

namespace th {
    class Bus {
    private:
        // 2kb
        std::array<uint8_t, 0x800> cpuRam = {0};
        std::shared_ptr<Cartridge> cart;

        // so system clock is my clock， also the ppu clock
        uint32_t nSystemClockCounter = 0;

    public:
        Bus();

        ~Bus();

    public:
        // Devices
        CPU cpu;
        PPU ppu;

        void clockCpu();

        uint8_t read(uint16_t addr, bool readOnly = false);

        void write(uint16_t addr, uint8_t data);

    public: // System Interface
        void insertCartridge(const std::shared_ptr<Cartridge> &cartridge);

        void reset();

        void clock();
    };

}
#endif //NES_BUS_H
