//
// Created by huhua on 2021/5/26.
//

#ifndef NES_CPU_SIMULATOR_H
#define NES_CPU_SIMULATOR_H


#include "../src/bus.h"
#include "cartridge.h"
#include "./base_app.h"
#include <map>
#define OLC_PGE_APPLICATION
#include "../src/olcPixelGameEngine.h"
#include "ppu/ui/ui_ppu.h"

class CPUSimulator: public BaseApp {
public:
    CPUSimulator();

private:
    bool bEmulationRun = false;
    float fResidualTime = 0.0f;
    int nSelectPalette = 0;
    // I need create you at runtime
    UIPPU *uiPPU = nullptr;

    void drawPalette(int y);

public:
    bool OnUserCreate() override;

    bool OnUserUpdate(float fElapsedTime) override;
};


#endif //NES_CPU_SIMULATOR_H
