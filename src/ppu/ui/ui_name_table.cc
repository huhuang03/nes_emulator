//
// Created by huhua on 2021/6/21.
//

#include "ui_name_table.h"
#include "../../util.h"

namespace th {
    void UINameTable::drawNo(olc::PixelGameEngine *engine, int x, int y) {
        for (int r = 0; r < nameTable->height; ++r) {
            for (int c = 0; c < nameTable->width; ++c) {
                auto index = r * nameTable->width + c;
                auto hexStr = hex(this->nameTable->data[index], 2);
                if (index == 0x82) {
//                    std::cout << "get from 0x82: " <<  hexStr << std::endl;
                    auto i = 1 + 1;
                }
                engine->DrawString(x + c * 16, y + r * 16, hexStr);
            }
        }
    }

    UINameTable::UINameTable(NameTable *nameTable) {
        this->nameTable = nameTable;
    }
}