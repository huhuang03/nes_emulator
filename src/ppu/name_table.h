//
// Created by huhua on 2021/6/21.
//

#ifndef NES_NAME_TABLE_H
#define NES_NAME_TABLE_H


class NameTable {
private:
    const int size_bytes = 1 * 1024;
    const int num_entries = 32;
    const int addr_min = 0x2000;
    const int addr_max = 0x3eff;
};


#endif //NES_NAME_TABLE_H
