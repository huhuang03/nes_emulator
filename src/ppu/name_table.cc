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
        this->data[addr & 0x3FF] = pData;
    }

}