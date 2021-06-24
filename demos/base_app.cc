//
// Created by huhua on 2021/6/8.
//

#include "base_app.h"

void BaseApp::drawRam(int x, int y, uint16_t nAddr, int nRows, int nColumns) {
    int nRamX = x, nRamY = y;
    for (int row = 0; row < nRows; row++) {
        std::string sOffsets = "$" + hex(nAddr, 4) + ":";
        for (int col = 0; col < nColumns; col++) {
            auto hexStr = hex(nes.read(nAddr++, true), 2);
            sOffsets += " " + hexStr;
        }
        DrawString(nRamX, nRamY, sOffsets);
        nRamY += 10;
    }
}

void BaseApp::drawCpu(int x, int y) {
    std::string status = "STATUS: ";
    DrawString(x, y, "STATUS:", olc::WHITE);
    DrawString(x + 64, y, "N", getFlagColor(CPU::N));
    DrawString(x + 80, y, "V", getFlagColor(CPU::V));
    DrawString(x + 96, y, "-", getFlagColor(CPU::U));
    DrawString(x + 112, y, "B", getFlagColor(CPU::B));
    DrawString(x + 128, y, "D", getFlagColor(CPU::D));
    DrawString(x + 144, y, "I", getFlagColor(CPU::I));
    DrawString(x + 160, y, "Z", getFlagColor(CPU::Z));
    DrawString(x + 178, y, "C", getFlagColor(CPU::C));

    DrawString(x, y + 10, "PC: $" + hex(nes.cpu.pc, 4));
    DrawString(x, y + 20, "A: $" + hex(nes.cpu.a, 2) + "  [" + std::to_string(nes.cpu.a) + "]");
    DrawString(x, y + 30, "X: $" + hex(nes.cpu.x, 2) + "  [" + std::to_string(nes.cpu.x) + "]");
    DrawString(x, y + 40, "Y: $" + hex(nes.cpu.y, 2) + "  [" + std::to_string(nes.cpu.y) + "]");
    DrawString(x, y + 50, "Stack P: $" + hex(nes.cpu.sp, 4));
}

void BaseApp::drawCode(int x, int y, int nLines) {
    this->drawCode(nes.cpu.pc, x, y, nLines);
}

std::string BaseApp::hex(uint32_t n, uint8_t d) {
    std::string s(d, '0');
    for (int i = d - 1; i >= 0; i--, n >>= 4) {
        s[i] = "0123456789ABCDEF"[n & 0xF];
    }
    return s;
}

olc::Pixel BaseApp::getFlagColor(CPU::FLAGS flag) {
    return nes.cpu.getFlag(flag)? olc::GREEN : olc::RED;
}

void BaseApp::drawCode(uint16_t pc, int x, int y, int nLines) {
    int startY = y;
    int endY = (nLines - 1) * 10 + y;

    int centerY = (nLines >> 1) * 10 + y;
    auto code = mapAsm.find(pc);
    if (code != mapAsm.end()) {
        DrawString(x, centerY, code->second, olc::CYAN);

        // draw above.
        auto pCode = code;
        pCode--;
        auto pY = centerY - 10;
        while (pY >= startY) {
            if (pCode != mapAsm.end()) {
                DrawString(x, pY, pCode->second);
                pCode--;
                pY -= 10;
            } else {
                break;
            }
        }

        // draw below
        pCode = code;
        pCode++;
        pY = centerY + 10;
        while (pY <= endY) {
            if (pCode != mapAsm.end()) {
                DrawString(x, pY, pCode->second);
                pCode++;
                pY += 10;
            } else {
                break;
            }
        }
    }
}
