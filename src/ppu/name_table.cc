//
// Created by huhua on 2021/6/21.
//

#include <iostream>
#include "name_table.h"
#include "../util.h"

namespace th {

    uint8_t NameTable::read(uint16_t addr) {
        return this->data[addr & 0x3FF];
    }

    void NameTable::write(uint16_t addr, uint8_t pData) {
        if (addr == 0x2082) {
            std::cout << "111write to name table 0x82" << hex(pData, 2) << std::endl;
        }
        this->data[addr & 0x3FF] = pData;
    }

}