//
// Created by huhua on 2021/6/5.
//

#ifndef NES_PPU_GUI_H
#define NES_PPU_GUI_H

#include "ppu/ppu.h"

/**
 * Yes, this display the ppu
 */
class PPUGui {
public:
    // it's strange that can implements here.
    explicit PPUGui(PPU *ppu);

private:
    PPU *ppu;
};


#endif //NES_PPU_GUI_H
