//
// Created by huhua on 2021/6/27.
//

#include <gtest/gtest.h>
#include <bus.h>
#include "empty_cartridge.h"

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <Bus.h>

TEST(CPUTEST, CompareCPU) {
    Bus right;
}

TEST(CPUTest, Test1) {
    Bus bus;

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
    bus.insertCartridge(std::make_shared<EmptyCartridge>());
    bus.cpu.loadByteCodeInHex("A2 0A 8E 00 00 A2 03 8E 01 00 AC 00 00 A9 00 18 6D 01 00 88 D0 FA 8D 02 00 EA EA EA");
    // fuck look like we need load a nes.
    bus.reset();
    CPU *cpu = &bus.cpu;
    ASSERT_EQ(cpu->x, 0);

//    LDX #10
    bus.clockCpu();
    ASSERT_EQ(cpu->x, 10);

//    STX $0000
    bus.clockCpu();
    ASSERT_EQ(cpu->read(0x0000), 10);

//    LDX #3
    bus.clockCpu();
    ASSERT_EQ(cpu->x, 3);

//    STX $0001
    bus.clockCpu();
    ASSERT_EQ(cpu->read(0x0001), 3);

//    LDY $0000
    bus.clockCpu();
    ASSERT_EQ(cpu->y, 10);

//    LDA #0
    bus.clockCpu();
    ASSERT_EQ(cpu->a, 0);

//    CLC
    bus.clockCpu();
    ASSERT_EQ(cpu->getFlag(CPU::C), 0);

//            loop
//    ADC $0001
    // a = a + mem[0x1] = 0 + 3
    bus.clockCpu();
    ASSERT_EQ(cpu->a, 3);

//    DEY
    bus.clockCpu();
    ASSERT_EQ(cpu->y, 9);
//    BNE loop
//    STA $0002
}