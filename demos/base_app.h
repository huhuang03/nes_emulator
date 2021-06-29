//
// Created by huhua on 2021/6/8.
//

#ifndef NES_BASE_APP_H
#define NES_BASE_APP_H

#include <bus.h>
#include "olcPixelGameEngine.h"

using namespace th;

class BaseApp : public olc::PixelGameEngine {

protected:
    Bus nes;
    std::shared_ptr<Cartridge> cate;
    std::map<uint16_t, std::string> mapAsm;

    olc::Pixel getFlagColor(CPU::FLAGS flag);
    std::string hex(uint32_t n, uint8_t d);

    void drawRam(int x, int y, uint16_t nAddr, int nRows, int nColumns);

    void drawCpu(int x, int y);

    void drawCode(int x, int y, int nLines);

    void drawCode(uint16_t pc, int x, int y, int nLines);


};


#endif //NES_BASE_APP_H
