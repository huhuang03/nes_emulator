//
// Created by huhua on 2021/6/21.
//

#ifndef NES_PPU_DEVICE_H
#define NES_PPU_DEVICE_H

class PPU;

class PPUDevice {
public:
    void setPPU(PPU *ppu1);

protected:
    PPU* ppu;
};


#endif //NES_PPU_DEVICE_H
