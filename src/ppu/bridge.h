//
// Created by huhua on 2021/6/21.
//

#ifndef NES_BRIDGE_H
#define NES_BRIDGE_H

#include <cstdint>

namespace th {
    class PPU;

/**
 * PPU has a bridge which can communicate with
 *
 * And bridge has the state?
 *
 * Who have the state is better??
 *
 * I think the ppu should have the state.
 */
    class Bridge {
    public:
        void setPPU(PPU *ppu);

        uint8_t read(uint16_t addr, bool readOnly = false);

        void write(uint16_t addr, uint8_t data);

    public:

    private:
        PPU *ppu = nullptr;

        union loopy_register {
            struct {
                uint16_t coarse_x: 5;
                uint16_t coarse_y: 5;
                uint16_t nametable_x: 1;
                uint16_t nametable_y: 1;
                uint16_t fine_y: 3;
                uint16_t unused: 1;
            };
            uint16_t reg = 0x0000;
        };

        loopy_register vram_addr;
        loopy_register tram_addr;
//        uint16_t ppu_address = 0x0000;
        // Address write to low byte
        uint8_t address_latch = 0x00;
        uint8_t ppu_data_buffer = 0x00;
        uint8_t fine_x = 0x00;

        void forward_ppu_address();
    };

}
#endif //NES_BRIDGE_H
