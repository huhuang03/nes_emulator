//
// Created by huhua on 2021/6/21.
//

#ifndef NES_NAME_TABLES_H
#define NES_NAME_TABLES_H

#include "./name_table.h"
#include "./ppu_device.h"

namespace th {

/**
 * https://wiki.nesdev.com/w/index.php/PPU_nametables
 * I have two real table, and two mirror table
 */
    class NameTables : public PPUDevice {

    public:
        const int addr_min = 0x2000;
        // 4 * 32 * 30
        const int addr_max = 0x2fff;

        const uint16_t t1_min = 0x2000;
        const uint16_t t1_max = 0x23ff;

        const uint16_t t2_min = 0x2400;
        const uint16_t t2_max = 0x27ff;

        const uint16_t t3_min = 0x2800;
        const uint16_t t3_max = 0x2dff;

        const uint16_t t4_min = 0x2c00;
        const uint16_t t4_max = 0x2fff;

        // haha, base friend is not work.
        friend class PPU;

    private:
        NameTable* getTable(uint16_t);

    protected:
        uint8_t read(uint16_t addr) override;

        void write(uint16_t addr, uint8_t data) override;

    public:
        NameTable t1;
        NameTable t2;

        void setChildPPU(PPU *ppu);
    };

}
#endif //NES_NAME_TABLES_H
