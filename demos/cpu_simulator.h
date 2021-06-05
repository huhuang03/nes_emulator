//
// Created by huhua on 2021/5/26.
//

#ifndef NES_CPU_SIMULATOR_H
#define NES_CPU_SIMULATOR_H


#define OLC_PGE_APPLICATION
#include "../src/olcPixelGameEngine.h"
#include "../src/bus.h"
#include "cartridge.h"
#include <map>

class CPUSimulator: public olc::PixelGameEngine {
public:
    CPUSimulator();

private:
    Bus nes;
    std::shared_ptr<Cartridge> cate;
    std::map<uint16_t, std::string> mapAsm;

    bool bEmulationRun = false;
    float fResidualTime = 0.0f;

    std::string hex(uint32_t n, uint8_t d);

    void drawRam(int x, int y, uint16_t nAddr, int nRows, int nColumns);

    void drawCpu(int x, int y);

    void drawCode(int x, int y, int nLines);

    olc::Pixel getFlagColor(CPU::FLAGS flag);

public:
    bool OnUserCreate() override;

    bool OnUserUpdate(float fElapsedTime) override;
};


#endif //NES_CPU_SIMULATOR_H
