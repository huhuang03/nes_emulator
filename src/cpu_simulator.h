//
// Created by huhua on 2021/5/26.
//

#ifndef NES_CPU_SIMULATOR_H
#define NES_CPU_SIMULATOR_H

#define OLC_PGE_APPLICATION

#include "olcPixelGameEngine.h"

class CpuSimulator: public olc::PixelGameEngine {
public:
    bool OnUserCreate() override;

    bool OnUserUpdate(float fElapsedTime) override;
};


#endif //NES_CPU_SIMULATOR_H
