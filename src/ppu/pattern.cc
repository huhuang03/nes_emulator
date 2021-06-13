//
// Created by huhua on 2021/6/7.
//

#include "pattern.h"
#include <string>
#include "ppu.h"

olc::Sprite &Pattern::getSprite(int which, int palette) {
    uint16_t b_start = (which == 0) ? table1_min : table2_min;
    for (int r_t = 0; r_t < num_tile; r_t++) {
        for (int c_t = 0; c_t < num_tile; c_t ++) {
            int i_t = c_t + r_t * num_tile;

            // now we located the tile
            for (int r_tp = 0; r_tp < 8; r_tp++) {
                uint16_t b_index = r_t * 16 * 8 * 2 + c_t * 8 * 2 + r_tp * 2;
                uint8_t l_byte = this->data[b_index + b_start];
                uint8_t h_byte = this->data[b_index + b_start + 1];

                for (int c_tp = 0; c_tp < 8; c_tp++) {
                    // 怎么说？
                    // 0为
                    uint8_t l_val = l_byte >> (8 - c_tp);
                    uint8_t h_val = h_byte >> (8 - c_tp);
                    uint8_t p_index = (l_val | h_val << 1) & 0x3;

                    int row = r_t * 16 * 8 + r_tp;
                    int col = c_t * 16 * 8 + c_tp;

                    this->sprites[which]->SetPixel(col, row, this->ppu->getColorInPalette(palette, p_index));
                }
            }
        }
    }
    return *this->sprites[which];
}

void Pattern::setPPU(PPU *pPpu) {
    this->ppu = pPpu;
}
