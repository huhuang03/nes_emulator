//
// Created by huhua on 2021/6/21.
//

#ifndef NES_NAME_TABLE_H
#define NES_NAME_TABLE_H


#include <cstdint>
#include "./ppu_device.h"

class UINameTable;

// I think a name table is 1kb.
// which is unit of 32 x 32
// And a unit is just a byte, 8 bit. which is 0 - 255
// How to use that byte, I didn't figure out for now.
class NameTable: PPUDevice {
public:
    const int width = 32;
    // strange height is only 30
    const int height = 30;


private:
    // look like we don't need 1024 byte. just 960(32x30) is enough
    // maybe the hardware is 1024.
    uint8_t data[1024];
    const int size_bytes = 1 * 1024;
    const int num_entries = 32;
    friend class UINameTable;

public:
    void write(uint16_t addr, uint8_t data);
    uint8_t read();
};



#endif //NES_NAME_TABLE_H
