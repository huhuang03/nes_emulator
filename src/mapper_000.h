//
// Created by hwf on 2021/6/4.
//

#ifndef NES_MAPPER_000_H
#define NES_MAPPER_000_H

#include "mapper.h"

namespace th {
    class Mapper000 : public Mapper {
    public:
        Mapper000(uint8_t prgBanks, uint8_t chrBanks);

    public:
        bool cpuMapRead(uint16_t addr, uint32_t &mapped_addr) override;

        bool cpuMapWrite(uint16_t addr, uint32_t &mapped_addr) override;

        bool ppuMapRead(uint16_t addr, uint32_t &mapped_addr) override;

        bool ppuMapWrite(uint16_t addr, uint32_t &mapped_addr) override;
    };

}
#endif //NES_MAPPER_000_H
