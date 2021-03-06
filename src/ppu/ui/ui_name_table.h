//
// Created by huhua on 2021/6/21.
//

#ifndef NES_UI_NAME_TABLE_H
#define NES_UI_NAME_TABLE_H

#include "../../olcPixelGameEngine.h"
#include "../name_table.h"

namespace th {
/**
 * UINameTable is private. So how can yo public?
 */
    class UINameTable {
    public:
        explicit UINameTable(NameTable *nameTable);

    private:
        NameTable *nameTable;

    public:
        void drawNo(olc::PixelGameEngine *engine, int x, int y);

        void draw(olc::PixelGameEngine *engine, int x, int y, int whichPattern, int whichPalette);

        void draw(olc::PixelGameEngine *engine, int x, int y, olc::Sprite* patternSprite);
    };

}
#endif //NES_UI_NAME_TABLE_H
