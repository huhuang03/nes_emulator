//
// Created by huhua on 2021/6/27.
//
#include <Cartridge.h>
#include <Bus.h>

#include <gtest/gtest.h>
#include <th_nes/bus.h>
#include "empty_cartridge.h"
#include <util.h>


#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

typedef std::map<std::uint16_t, std::string> CodeMap;

void printCodeMap(CodeMap& codeMap) {
    for (auto & it : codeMap) {
        std::cout << hex(it.first, 4) << ": " << it.second << std::endl;
    }
}

void compareCpu(Bus &cpu1, th::Bus &cpu2) {
    for (uint32_t addr = 0; addr <= 0xFFFF; addr++) {
        uint16_t tmpAddr = addr;
        auto data1 = cpu1.cpuRead(tmpAddr, true);
        auto data2 = cpu2.read(tmpAddr, true);
        // why you not error right now.
        ASSERT_EQ(data1, data2) << ", data1: " << hex(data1, 2) << " at pc: " << hex(cpu1.cpu.pc, 4) << ", addr: " << hex(addr, 4);
    }
    ASSERT_EQ(cpu1.cpu.pc, cpu2.cpu.pc) << " at pc: " << hex(cpu1.cpu.pc, 4);
    ASSERT_EQ(cpu1.cpu.x, cpu2.cpu.x) << " at pc: " << hex(cpu1.cpu.pc, 4);
    ASSERT_EQ(cpu1.cpu.y, cpu2.cpu.y) << " at pc: " << hex(cpu1.cpu.pc, 4);
    ASSERT_EQ(cpu1.cpu.a, cpu2.cpu.a) << " at pc: " << hex(cpu1.cpu.pc, 4);
}

// 大小写好像不敏感啊。

TEST(CPUTEST, CompareCPU) {
    std::string nesPath = "../assets/nestest.nes";
    Bus rightBus;
    rightBus.insertCartridge(std::make_shared<Cartridge>(nesPath));
    rightBus.reset();

    CodeMap rightAssemble = rightBus.cpu.disassemble(0x0, 0xFFFF);

    th::Bus compareBus;
    compareBus.insertCartridge(std::make_shared<th::Cartridge>(nesPath));
    compareBus.reset();

    std::map<std::uint16_t, std::string> compareAssemble = compareBus.cpu.disassemble(0x0, 0xFFFF);

    std::cout << "cpu1, 0xc291: " << rightAssemble.find(0xC291)->second << std::endl;
    std::cout << "cpu2, 0xc291: " << compareAssemble.find(0xC291)->second << std::endl;
    return;

    auto it1 = rightAssemble.begin();
    auto it2 = compareAssemble.begin();

    while (it1 != rightAssemble.end()) {
        ASSERT_EQ(it1->second, it2->second);
        it1++;
        it2++;
//        std::cout << it1->first << std::endl;
    }

    uint32_t max = 0xffffff;

    int i = 0;
    auto pc1  = rightBus.cpu.pc;
    auto pc2 = compareBus.cpu.pc;
    ASSERT_EQ(pc1, pc2);

    do {
        std::cout << "before clock pc: " << hex(rightBus.cpu.pc, 4) <<  std::endl;
        rightBus.clockCpu();
        compareBus.clockCpu();
        pc1  = rightBus.cpu.pc;
        pc2 = compareBus.cpu.pc;
        ASSERT_EQ(pc1, pc2) << "pc1: " << hex(pc1, 4) << ", pc2: " << hex(pc2, 4);
        // why you not stop??
        i++;
    } while (i < max);

}

TEST(CPUTEST, TestCode) {
    th::Bus bus;
    bus.insertCartridge(std::make_shared<th::EmptyCartridge>());
    // let try a code.

    /*
     * ADC 我是不是还得测多种模式啊？？
     *
     */

}

TEST(CPUTest, Test1) {
    th::Bus bus;

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
    bus.insertCartridge(std::make_shared<th::EmptyCartridge>());
    bus.cpu.loadByteCodeInHex("A2 0A 8E 00 00 A2 03 8E 01 00 AC 00 00 A9 00 18 6D 01 00 88 D0 FA 8D 02 00 EA EA EA");
    // fuck look like we need load a nes.
    bus.reset();
    th::CPU *cpu = &bus.cpu;
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
    ASSERT_EQ(cpu->getFlag(th::CPU::C), 0);

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