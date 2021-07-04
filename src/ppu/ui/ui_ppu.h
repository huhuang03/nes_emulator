//
// Created by huhua on 2021/6/23.
//

#ifndef NES_UI_PPU_H
#define NES_UI_PPU_H

#include "../../olcPixelGameEngine.h"
#include "../ppu.h"
#include "./ui_name_table.h"

namespace th {
/**
 * I'm tired of you. so I want just open something to ui.
 */
    class UIPPU {
    public:
        // let you inline
        UIPPU(PPU *ppu);

    private:
        PPU *ppu;

    public:
        UINameTable *uiNameTable1;
        UINameTable *uiNameTable2;
        void drawNameTable1No(olc::PixelGameEngine *engine, int x, int y);

        void drawNameTable2No(olc::PixelGameEngine *engine, int x, int y);

        void drawNameTable(olc::PixelGameEngine *engine, int x, int y, int whichNameTable, int whichPattern, int whichPalette);

        void drawNameTable(olc::PixelGameEngine *engine, int x, int y, int whichNameTable, olc::Sprite *patternSprite);
    };

}
#endif //NES_UI_PPU_H
