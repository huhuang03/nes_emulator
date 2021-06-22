//
// Created by huhua on 2021/6/21.
//

#include "name_table.h"

uint8_t NameTable::read(uint16_t addr) {
    return this->data[addr & 0x400];
}

void NameTable::write(uint16_t addr, uint8_t data) {
    this->data[addr & 0x400] = data;
}

