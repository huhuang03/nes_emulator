//
// Created by huhua on 2021/7/4.
//

#include <th_nes/bus.h>
#include <iostream>
#include <fstream>

#define OLC_PGE_APPLICATION
#include "../src/olcPixelGameEngine.h"

static std::string hex(uint32_t n, uint8_t d) {
    std::string s(d, '0');
    for (int i = d - 1; i >= 0; i--, n >>= 4) {
        s[i] = "0123456789ABCDEF"[n & 0xF];
    }
    return s;
}

int main() {
    th::Bus nes;
    auto cate = std::make_shared<th::Cartridge>("../assets/nestest.nes");
    nes.insertCartridge(cate);

    nes.reset();

    auto mapAsm = nes.cpu.disassemble(0xc004, 0xFFFF);

    std::string filePath = "dump.txt";
    std::ofstream f;
    f.open(filePath);
    for (auto &code: mapAsm) {
        f << hex(code.first, 4 ) << ": " << code.second << std::endl;
    }
//    f << "Hello in ofstream." << std::endl;
    f.close();
    return 0;
}
