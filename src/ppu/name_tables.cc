//
// Created by huhua on 2021/6/21.
//

#include "name_tables.h"
#include "./ppu.h"
#include "../util.h"

namespace th {

    uint8_t NameTables::read(uint16_t addr) {
        return getTable(addr).read(addr);
    }

    void NameTables::write(uint16_t addr, uint8_t data) {
        auto table = getTable(addr);
        table.write(addr, data);
        if (&table == &this->t1) {
            std::cout << "write to table1 called" << std::endl;
        }
    }

    NameTable &NameTables::getTable(uint16_t addr) {
        auto in_t1 = in(addr, t1_min, t1_max);
        auto in_t2 = in(addr, t2_min, t2_max);
        auto in_t3 = in(addr, t3_min, t3_max);
        auto in_t4 = in(addr, t4_min, t4_max);

        if (ppu->cart->isHorizontal()) {
            if (in_t1 || in_t2) {
                return this->t1;
            } else {
                return this->t2;
            }
        } else if (ppu->cart->isVertical()) {
            // t1 t2
            // mi_t1 mi_t2
            if (in_t1 || in_t3) {
                return this->t1;
            } else {
                return this->t2;
            }
        }
    }

}