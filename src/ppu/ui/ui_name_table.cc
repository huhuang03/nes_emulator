//
// Created by huhua on 2021/6/21.
//

#include "ui_name_table.h"
#include "../../util.h"

void UINameTable::drawNo(olc::PixelGameEngine *engine, int x, int y) {
    for (int r = 0; r < nameTable->height; ++r) {
        for (int c = 0; c < nameTable->width; ++c) {
            engine->DrawString(x * 16, y * 16, hex(this->nameTable->data[r * nameTable->width + c], 2));
        }
    }
}

UINameTable::UINameTable(NameTable *nameTable) {
    this->nameTable = nameTable;
}
