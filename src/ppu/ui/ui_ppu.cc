//
// Created by huhua on 2021/6/23.
//

#include "ui_ppu.h"

void UIPPU::drawNameTable1No(olc::PixelGameEngine *engine, int x, int y) {
    this->uiNameTable1->drawNo(engine, x, y);
}

void UIPPU::drawNameTable2No(olc::PixelGameEngine *engine, int x, int y) {
    this->uiNameTable2->drawNo(engine, x, y);
}

// don't complain about the param name?
UIPPU::UIPPU(PPU *ppu) {
    this->ppu = ppu;
    this->uiNameTable1 = new UINameTable(&this->ppu->nameTables.t1);
    this->uiNameTable2 = new UINameTable(&this->ppu->nameTables.t2);
}
