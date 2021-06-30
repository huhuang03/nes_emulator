//
// Created by huhua on 2021/6/21.
//

#ifndef NES_NAME_TABLE_H
#define NES_NAME_TABLE_H


#include <cstdint>
#include "./ppu_device.h"

namespace th {
    class UINameTable;

    class NameTables;

// I think a name table is 1kb.
// which is unit of 32 x 32
// And a unit is just a byte, 8 bit. which is 0 - 255
// A unit is an id in pattern table
    class NameTable : PPUDevice {
    public:
        const int width = 32;
        // strange height is only 30
        const int height = 30;

        friend class NameTables;

    private:
        uint8_t data[1024] = {0};

        friend class UINameTable;

        uint8_t read(uint16_t addr) override;

        void write(uint16_t addr, uint8_t data) override;
    };

}
#endif //NES_NAME_TABLE_H
