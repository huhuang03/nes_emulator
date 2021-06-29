//
// Created by huhua on 2021/6/21.
//

#ifndef NES_UI_NAME_TABLE_H
#define NES_UI_NAME_TABLE_H

#include "../../olcPixelGameEngine.h"
#include "../name_table.h"


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
};


#endif //NES_UI_NAME_TABLE_H
