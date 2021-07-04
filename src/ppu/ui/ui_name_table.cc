//
// Created by huhua on 2021/6/21.
//

#include "ui_name_table.h"
#include "../../util.h"
#include "../../ppu/ppu.h"

namespace th {
    void UINameTable::drawNo(olc::PixelGameEngine *engine, int x, int y) {
        for (int r = 0; r < nameTable->height; ++r) {
            for (int c = 0; c < nameTable->width; ++c) {
                auto index = r * nameTable->width + c;
                auto hexStr = hex(this->nameTable->data[index], 2);
                engine->DrawString(x + c * 16, y + r * 16, hexStr);
            }
        }
    }

    UINameTable::UINameTable(NameTable *nameTable) {
        this->nameTable = nameTable;
    }

    void UINameTable::draw(olc::PixelGameEngine *engine, int x, int y, int whichPattern, int whichPalette) {
        auto pattern = this->nameTable->ppu->getPattern(whichPattern, whichPalette);
        this->draw(engine, x, y, pattern);
    }

    void UINameTable::draw(olc::PixelGameEngine *engine, int x, int y, olc::Sprite *pattern) {
        for (int r = 0; r < nameTable->height; ++r) {
            for (int c = 0; c < nameTable->width; ++c) {

                auto index = r * nameTable->width + c;
                auto patternIndex = this->nameTable->read(index);

                int r1 = patternIndex & 0xF;
                int c1 = (patternIndex >> 0x4) & 0xF;
                engine->DrawPartialSprite(x + c * 16, y + r * 16, pattern, r1 << 3, c1 << 3, 8, 8, 2);
            }
        }
    }
}