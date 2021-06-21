//
// Created by huhua on 2021/6/21.
//

#ifndef NES_BRIDGE_H
#define NES_BRIDGE_H

#include <cstdint>

class PPU;

/**
 * PPU has a bridge which can communicate with
 */
class Bridge {
public:
    void setPPU(PPU *ppu);
    uint8_t read(uint16_t addr, bool readOnly = false);
    void write(uint16_t addr, uint8_t data);

public:
    union {
        struct {
            uint8_t grayscale: 1;
            uint8_t render_background_left: 1;
            uint8_t render_sprites_left: 1;
            uint8_t render_background: 1;
            uint8_t render_sprites: 1;
            uint8_t enhance_red: 1;
            uint8_t enhance_green: 1;
            uint8_t enhance_blue: 1;
        };
        uint8_t reg;
    } mask{};

    union {
        struct {
            uint8_t unused: 5;
            uint8_t sprite_overflow: 1;
            uint8_t sprite_zero_hit: 1;
            // In the video, It seems says that when scan the out of the height, means vertical_blank
            uint8_t vertical_blank: 1;
        };
        uint8_t reg;
    } status{};

    union {
        struct {
            uint8_t nametable_x: 1;
            uint8_t nametable_y: 1;
            uint8_t increment_mode: 1;
            uint8_t pattern_sprite: 1;
            uint8_t pattern_background: 1;
            uint8_t sprite_size: 1;
            uint8_t slave_mode: 1;
            uint8_t enable_nmi: 1;
        };
        uint8_t reg;
    } control{};


private:
    PPU *ppu;

    uint16_t ppu_address = 0x0000;

    // Address write to low byte
    uint8_t address_latch = 0x00;
    uint8_t ppu_data_buffer = 0x00;

    void forward_ppu_address();
};


#endif //NES_BRIDGE_H
