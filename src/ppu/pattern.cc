//
// Created by huhua on 2021/6/7.
//

#include "pattern.h"
#include <string>
#include "ppu.h"

namespace th {
    olc::Sprite *Pattern::getSprite(int which, int palette) {
        uint16_t b_start = (which == 0) ? table1_min : table2_min;
        for (int r_t = 0; r_t < num_tile; r_t++) {
            for (int c_t = 0; c_t < num_tile; c_t++) {
                // now we located the tile
                for (int r_tp = 0; r_tp < 8; r_tp++) {
                    uint16_t b_index = (r_t * num_tile + c_t) * 16 + r_tp;
                    uint8_t l_byte = ppu->ppuRead(b_index + b_start);
                    uint8_t h_byte = ppu->ppuRead(b_index + b_start + 8);

                    for (int c_tp = 0; c_tp < 8; c_tp++) {
                        uint8_t l_val = l_byte >> (7 - c_tp);
                        uint8_t h_val = h_byte >> (7 - c_tp);
                        // strange is plus.

                        uint8_t p_index = (l_val + h_val) & 0b11;
//                    uint8_t p_index = (l_val | h_val << 1) & 0b11;

                        int row = r_t * 8 + r_tp;
                        int col = c_t * 8 + c_tp;

                        this->sprites[which]->SetPixel(col, row, this->ppu->getColorInPalette(palette, p_index));
                    }
                }
            }
        }
        return this->sprites[which];
    }

    void Pattern::setPPU(PPU *pPpu) {
        this->ppu = pPpu;
    }
}
