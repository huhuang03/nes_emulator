//
// Created by huhua on 2021/5/26.
//

#include "cpu_debugger.h"
#include "cpu_simulator.h"


bool CPUDebugger::OnUserCreate() {
    // Load Program (assembled at https://www.masswerk.at/6502/assembler.html)
    /*
        *=$8000
        LDX #10
        STX $0000
        LDX #3
        STX $0001
        LDY $0000
        LDA #0
        CLC
        loop
        ADC $0001
        DEY
        BNE loop
        STA $0002
        NOP
        NOP
        NOP
    */
    std::stringstream ss;
    ss << "A2 0A 8E 00 00 A2 03 8E 01 00 AC 00 00 A9 00 18 6D 01 00 88 D0 FA 8D 02 00 EA EA EA";
    uint16_t nOffset = 0x8000;
    while (!ss.eof()) {
        // yes, this will jump space.
        std::string b;
        ss >> b;
        nes.write(nOffset++, (uint8_t) std::stol(b, nullptr, 16));
    }
    nes.write(0xFFFC, 0x00);
    nes.write(0xFFFD, 0x80);

    // seems block at here??
    mapAsm = nes.cpu.disassemble(0x0000, 0xFFFF);
    nes.cpu.reset();
    return true;
}

bool CPUDebugger::OnUserUpdate(float fElapsedTime) {
    Clear(olc::DARK_BLUE);

    if (GetKey(olc::Key::SPACE).bPressed) {
        do {
            nes.cpu.clock();
        } while (!nes.cpu.complete());
    }

    if (GetKey(olc::Key::R).bPressed)
        nes.cpu.reset();

    if (GetKey(olc::Key::I).bPressed)
        nes.cpu.irq();

    if (GetKey(olc::Key::N).bPressed)
        nes.cpu.nmi();

    drawRam(2, 2, 0x0000, 16, 16);
    drawRam(2, 182, 0x8000, 16, 16);
    drawCpu(448, 2);
    drawCode(448, 72, 26);

    DrawString(10, 370, "SPACE = Step Instruction    R = RESET    I = IRQ    N = NMI");
    return true;
}

std::string CPUDebugger::hex(uint32_t n, uint8_t d) {
    std::string s(d, '0');
    for (int i = d - 1; i >= 0; i--, n >>= 4) {
        s[i] = "0123456789ABCDEF"[n & 0xF];
    }
    return s;
}

void CPUDebugger::drawRam(int x, int y, uint16_t nAddr, int nRows, int nColumns) {
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

void CPUDebugger::drawCpu(int x, int y) {
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

olc::Pixel CPUDebugger::getFlagColor(CPU::FLAGS flag) {
    return nes.cpu.getFlag(flag)? olc::GREEN : olc::RED;
}

void CPUDebugger::drawCode(int x, int y, int nLines) {
    int startY = y;
    int endY = (nLines - 1) * 10 + y;

    int centerY = (nLines >> 1) * 10 + y;
    auto code = mapAsm.find(nes.cpu.pc);
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


///**
// * Not work for now, because we must plugin cartridge.
// * May be sometimes later I can figure out how to do this.
// */
//int main() {
//    CpuSimulator simulator;
//    simulator.Construct(680, 480, 2, 2);
//    simulator.Start();
//    return 0;
//}
