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
    drawPalette(340);

    DrawSprite(0, 0, &nes.ppu.GetScreen(), 2);

    DrawString(10, 370, "SPACE = Step Instruction    R = RESET    I = IRQ    N = NMI");
    return true;
}


CPUSimulator::CPUSimulator() {
    sAppName = "CPU Simulator";
}

void CPUSimulator::drawPalette(int y) {
    const int nSwatchSize = 6;
    for (int palette = 0; palette < 8; palette++) {
        for (int index = 0; index < 3; index++) {
            auto color = nes.ppu.getColorInPalette(palette, index);
            FillRect(516 + (palette * 5 + index) * nSwatchSize, y, nSwatchSize, nSwatchSize, color);
        }
    }
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