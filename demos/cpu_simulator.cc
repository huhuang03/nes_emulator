//
// Created by huhua on 2021/5/26.
//

#include "cpu_simulator.h"

bool CPUSimulator::OnUserCreate() {
    uiPPU = new UIPPU(&nes.ppu);
    cate = std::make_shared<Cartridge>("../assets/nestest.nes");
    nes.insertCartridge(cate);

    mapAsm = nes.cpu.disassemble(0x0000, 0xFFFF);
    nes.reset();

    // run to 0xc037?
    while (nes.cpu.pc != 0xc037) {
        nes.clockCpu();
    }

    return true;
}

bool CPUSimulator::OnUserUpdate(float fElapsedTime) {
    Clear(olc::DARK_BLUE);

    if (GetKey(olc::Key::P).bReleased) {
        (++nSelectPalette) &= 0x7;
    }


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
            // dry out previous
            nes.clockCpu();
        }

        if (GetKey(olc::Key::F).bPressed) {
            do {
                nes.clock();
            } while (!nes.ppu.frame_complete);

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
//    drawCode(0xc037, 516, 72, 26);
    drawPalette(340);

    DrawSprite(516, 348, &nes.ppu.getPattern(0, nSelectPalette));
    DrawSprite(648, 348, &nes.ppu.getPattern(1, nSelectPalette));


    // DrawSprite(0, 0, &nes.ppu.GetScreen(), 2);
    uiPPU->drawNameTable1No(this, 0, 0);

//    DrawString(10, 370, "SPACE = Switch MOde    P = Palette    R = Reset");
    return true;
}


CPUSimulator::CPUSimulator() {
    sAppName = "CPU Simulator";
}

void CPUSimulator::drawPalette(int y) {
    const int nSwatchSize = 6;
    for (int palette = 0; palette < 8; palette++) {
        for (int index = 0; index < 4; index++) {
            auto color = nes.ppu.getColorInPalette(palette, index);
            // now our goal is the right palette.
            FillRect(516 + (palette * 5 + index) * nSwatchSize, y, nSwatchSize, nSwatchSize, color);
        }
    }

    DrawRect(516 + nSelectPalette * (nSwatchSize * 5) - 1, 339, (nSwatchSize * 4), nSwatchSize, olc::WHITE);
}


/**
 * Not work for now, because we must plugin cartridge.
 * May be sometimes later I can figure out how to do this.
 */
int main() {
    CPUSimulator simulator;
    simulator.Construct(780, 480, 2, 2);
    simulator.Start();
    return 0;
}
