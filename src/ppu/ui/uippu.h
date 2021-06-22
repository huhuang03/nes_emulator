//
// Created by huhua on 2021/6/23.
//

#ifndef NES_UIPPU_H
#define NES_UIPPU_H

#include <olcPixelGameEngine.h>
#include "../ppu.h"

/**
 * I'm tired of you. so I want just open something to ui.
 */
class UIPPU {
public:
    // let you inline
    UIPPU(PPU *ppu): ppu(ppu) {};

private:
    PPU *ppu;

public:
    void drawNameTable1No(olc::PixelGameEngine *engine, int x, int y);

    void drawNameTable2No(olc::PixelGameEngine *engine, int x, int y);
};


#endif //NES_UIPPU_H
