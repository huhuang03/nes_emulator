//
// Created by huhua on 2021/6/21.
//

#ifndef NES_PPU_DEVICE_H
#define NES_PPU_DEVICE_H

#include <cstdint>

class PPU;

class PPUDevice {
public:
    void setPPU(PPU *ppu1);

protected:
    PPU* ppu = nullptr;
    virtual uint8_t read(uint16_t addr) = 0;
    virtual void write(uint16_t addr, uint8_t data) = 0;
    friend class PPU;
};


#endif //NES_PPU_DEVICE_H
