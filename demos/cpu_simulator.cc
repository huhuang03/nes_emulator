//
// Created by huhua on 2021/5/26.
//

#include "cpu_simulator.h"

bool CPUSimulator::OnUserCreate() {
    cate = std::make_shared<Cartridge>("../assets/nestest.nes");
    nes.insertCartridge(cate);

    mapAsm = nes.cpu.disassemble(0x0000, 0xFFFF);

    nes.reset();
    return true;
}

bool CPUSimulator::OnUserUpdate(float fElapsedTime) {
    Clear(olc::DARK_BLUE);

    if (bEmulationRun) {
        if (fResidualTime > 0.0f) {
            fResidualTime -= fElapsedTime;
        } else {
            fResidualTime += (1.0f / 60.0f) - fElapsedTime;
            do {nes.clock();} while (!nes.ppu.frame_complete);
            nes.ppu.frame_complete = false;
        }
    } else {
        if (GetKey(olc::Key::C).bPressed) {
            do {
                nes.cpu.clock();
            } while (!nes.cpu.complete());

            // why the tutorial do this clock again? I can't figure out now.
            do {
                nes.clock();
            } while (nes.cpu.complete());
        }

        if (GetKey(olc::Key::F).bPressed) {
            do {
                nes.cpu.clock();
            } while (!nes.ppu.frame_complete);

            // why the tutorial do this clock again? I can't figure out now.
            do {
                nes.clock();
            } while (!nes.cpu.complete());
            nes.ppu.frame_complete = false;
        }

    }

    if (GetKey(olc::Key::R).bPressed)
        nes.cpu.reset();

    if (GetKey(olc::Key::SPACE).bPressed)
        bEmulationRun = !bEmulationRun;

    drawCpu(516, 2);
    drawCode(516, 72, 26);
    DrawSprite(0, 0, &nes.ppu.GetScreen(), 2);

//    FillRect()

    DrawString(10, 370, "SPACE = Step Instruction    R = RESET    I = IRQ    N = NMI");
    return true;
}

std::string CPUSimulator::hex(uint32_t n, uint8_t d) {
    std::string s(d, '0');
    for (int i = d - 1; i >= 0; i--, n >>= 4) {
        s[i] = "0123456789ABCDEF"[n & 0xF];
    }
    return s;
}

void CPUSimulator::drawRam(int x, int y, uint16_t nAddr, int nRows, int nColumns) {
    int nRamX = x, nRamY = y;
    for (int row = 0; row < nRows; row++) {
        std::string sOffsets = "$" + hex(nAddr, 4) + ":";
        for (int col = 0; col < nColumns; col++) {
            auto hexStr = hex(nes.cpuRead(nAddr++, true), 2);
            sOffsets += " " + hexStr;
        }
        DrawString(nRamX, nRamY, sOffsets);
        nRamY += 10;
    }
}

void CPUSimulator::drawCpu(int x, int y) {
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

olc::Pixel CPUSimulator::getFlagColor(CPU::FLAGS flag) {
    return nes.cpu.getFlag(flag)? olc::GREEN : olc::RED;
}

void CPUSimulator::drawCode(int x, int y, int nLines) {
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

CPUSimulator::CPUSimulator() {
    sAppName = "CPU Simulator";
}


/**
 * Not work for now, because we must plugin cartridge.
 * May be sometimes later I can figure out how to do this.
 */
int main() {
    CPUSimulator simulator;
    simulator.Construct(680, 480, 2, 2);
    simulator.Start();
    return 0;
}
