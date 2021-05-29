//
// Created by huhua on 2021/5/26.
//

#ifndef NES_CPU_SIMULATOR_H
#define NES_CPU_SIMULATOR_H


#include "olcPixelGameEngine.h"
#include "./bus.h"
#include <map>

class CpuSimulator: public olc::PixelGameEngine {
private:
    Bus nes;
    std::map<uint16_t, std::string> mapAsm;

    std::string hex(uint32_t n, uint8_t d);

    void drawRam(int x, int y, uint16_t nAddr, int nRows, int nColumns);

    void drawCpu(int x, int y);

    olc::Pixel getFlagColor(CPU::FLAGS flag);

public:
    bool OnUserCreate() override;

    bool OnUserUpdate(float fElapsedTime) override;
};


#endif //NES_CPU_SIMULATOR_H
