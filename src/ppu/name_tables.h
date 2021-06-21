//
// Created by huhua on 2021/6/21.
//

#ifndef NES_NAME_TABLES_H
#define NES_NAME_TABLES_H

#include "./name_table.h"
#include "./ppu_device.h"

class NameTables: public PPUDevice {

public:
    const int addr_min = 0x2000;

    // 4 * 32 * 30
    const int addr_max = 0x3eff;

public:
    NameTable t1;
    NameTable t2;
};


#endif //NES_NAME_TABLES_H
