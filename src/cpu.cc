//
// Created by huhua on 2021/5/22.
//

#include "th_nes/cpu.h"
#include "include/th_nes/bus.h"
#include <stdexcept>
#include <iostream>

namespace th {
    CPU::CPU() {
        using a = CPU;
        lookup = {
                {"BRK", &a::BRK, &a::IMM, 7},
                {"ORA", &a::ORA, &a::IZX, 6},
                {"???", &a::XXX, &a::IMP, 2},
                {"???", &a::XXX, &a::IMP, 8},
                {"???", &a::NOP, &a::IMP, 3},
                {"ORA", &a::ORA, &a::ZP0, 3},
                {"ASL", &a::ASL, &a::ZP0, 5},
                {"???", &a::XXX, &a::IMP, 5},
                {"PHP", &a::PHP, &a::IMP, 3},
                {"ORA", &a::ORA, &a::IMM, 2},
                {"ASL", &a::ASL, &a::IMP, 2},
                {"???", &a::XXX, &a::IMP, 2},
                {"???", &a::NOP, &a::IMP, 4},
                {"ORA", &a::ORA, &a::ABS, 4},
                {"ASL", &a::ASL, &a::ABS, 6},
                {"???", &a::XXX, &a::IMP, 6},
                {"BPL", &a::BPL, &a::REL, 2},
                {"ORA", &a::ORA, &a::IZY, 5},
                {"???", &a::XXX, &a::IMP, 2},
                {"???", &a::XXX, &a::IMP, 8},
                {"???", &a::NOP, &a::IMP, 4},
                {"ORA", &a::ORA, &a::ZPX, 4},
                {"ASL", &a::ASL, &a::ZPX, 6},
                {"???", &a::XXX, &a::IMP, 6},
                {"CLC", &a::CLC, &a::IMP, 2},
                {"ORA", &a::ORA, &a::ABY, 4},
                {"???", &a::NOP, &a::IMP, 2},
                {"???", &a::XXX, &a::IMP, 7},
                {"???", &a::NOP, &a::IMP, 4},
                {"ORA", &a::ORA, &a::ABX, 4},
                {"ASL", &a::ASL, &a::ABX, 7},
                {"???", &a::XXX, &a::IMP, 7},
                {"JSR", &a::JSR, &a::ABS, 6},
                {"AND", &a::AND, &a::IZX, 6},
                {"???", &a::XXX, &a::IMP, 2},
                {"???", &a::XXX, &a::IMP, 8},
                {"BIT", &a::BIT, &a::ZP0, 3},
                {"AND", &a::AND, &a::ZP0, 3},
                {"ROL", &a::ROL, &a::ZP0, 5},
                {"???", &a::XXX, &a::IMP, 5},
                {"PLP", &a::PLP, &a::IMP, 4},
                {"AND", &a::AND, &a::IMM, 2},
                {"ROL", &a::ROL, &a::IMP, 2},
                {"???", &a::XXX, &a::IMP, 2},
                {"BIT", &a::BIT, &a::ABS, 4},
                {"AND", &a::AND, &a::ABS, 4},
                {"ROL", &a::ROL, &a::ABS, 6},
                {"???", &a::XXX, &a::IMP, 6},
                {"BMI", &a::BMI, &a::REL, 2},
                {"AND", &a::AND, &a::IZY, 5},
                {"???", &a::XXX, &a::IMP, 2},
                {"???", &a::XXX, &a::IMP, 8},
                {"???", &a::NOP, &a::IMP, 4},
                {"AND", &a::AND, &a::ZPX, 4},
                {"ROL", &a::ROL, &a::ZPX, 6},
                {"???", &a::XXX, &a::IMP, 6},
                {"SEC", &a::SEC, &a::IMP, 2},
                {"AND", &a::AND, &a::ABY, 4},
                {"???", &a::NOP, &a::IMP, 2},
                {"???", &a::XXX, &a::IMP, 7},
                {"???", &a::NOP, &a::IMP, 4},
                {"AND", &a::AND, &a::ABX, 4},
                {"ROL", &a::ROL, &a::ABX, 7},
                {"???", &a::XXX, &a::IMP, 7},
                {"RTI", &a::RTI, &a::IMP, 6},
                {"EOR", &a::EOR, &a::IZX, 6},
                {"???", &a::XXX, &a::IMP, 2},
                {"???", &a::XXX, &a::IMP, 8},
                {"???", &a::NOP, &a::IMP, 3},
                {"EOR", &a::EOR, &a::ZP0, 3},
                {"LSR", &a::LSR, &a::ZP0, 5},
                {"???", &a::XXX, &a::IMP, 5},
                {"PHA", &a::PHA, &a::IMP, 3},
                {"EOR", &a::EOR, &a::IMM, 2},
                {"LSR", &a::LSR, &a::IMP, 2},
                {"???", &a::XXX, &a::IMP, 2},
                {"JMP", &a::JMP, &a::ABS, 3},
                {"EOR", &a::EOR, &a::ABS, 4},
                {"LSR", &a::LSR, &a::ABS, 6},
                {"???", &a::XXX, &a::IMP, 6},
                {"BVC", &a::BVC, &a::REL, 2},
                {"EOR", &a::EOR, &a::IZY, 5},
                {"???", &a::XXX, &a::IMP, 2},
                {"???", &a::XXX, &a::IMP, 8},
                {"???", &a::NOP, &a::IMP, 4},
                {"EOR", &a::EOR, &a::ZPX, 4},
                {"LSR", &a::LSR, &a::ZPX, 6},
                {"???", &a::XXX, &a::IMP, 6},
                {"CLI", &a::CLI, &a::IMP, 2},
                {"EOR", &a::EOR, &a::ABY, 4},
                {"???", &a::NOP, &a::IMP, 2},
                {"???", &a::XXX, &a::IMP, 7},
                {"???", &a::NOP, &a::IMP, 4},
                {"EOR", &a::EOR, &a::ABX, 4},
                {"LSR", &a::LSR, &a::ABX, 7},
                {"???", &a::XXX, &a::IMP, 7},
                {"RTS", &a::RTS, &a::IMP, 6},
                {"ADC", &a::ADC, &a::IZX, 6},
                {"???", &a::XXX, &a::IMP, 2},
                {"???", &a::XXX, &a::IMP, 8},
                {"???", &a::NOP, &a::IMP, 3},
                {"ADC", &a::ADC, &a::ZP0, 3},
                {"ROR", &a::ROR, &a::ZP0, 5},
                {"???", &a::XXX, &a::IMP, 5},
                {"PLA", &a::PLA, &a::IMP, 4},
                {"ADC", &a::ADC, &a::IMM, 2},
                {"ROR", &a::ROR, &a::IMP, 2},
                {"???", &a::XXX, &a::IMP, 2},
                {"JMP", &a::JMP, &a::IND, 5},
                {"ADC", &a::ADC, &a::ABS, 4},
                {"ROR", &a::ROR, &a::ABS, 6},
                {"???", &a::XXX, &a::IMP, 6},
                {"BVS", &a::BVS, &a::REL, 2},
                {"ADC", &a::ADC, &a::IZY, 5},
                {"???", &a::XXX, &a::IMP, 2},
                {"???", &a::XXX, &a::IMP, 8},
                {"???", &a::NOP, &a::IMP, 4},
                {"ADC", &a::ADC, &a::ZPX, 4},
                {"ROR", &a::ROR, &a::ZPX, 6},
                {"???", &a::XXX, &a::IMP, 6},
                {"SEI", &a::SEI, &a::IMP, 2},
                {"ADC", &a::ADC, &a::ABY, 4},
                {"???", &a::NOP, &a::IMP, 2},
                {"???", &a::XXX, &a::IMP, 7},
                {"???", &a::NOP, &a::IMP, 4},
                {"ADC", &a::ADC, &a::ABX, 4},
                {"ROR", &a::ROR, &a::ABX, 7},
                {"???", &a::XXX, &a::IMP, 7},
                {"???", &a::NOP, &a::IMP, 2},
                {"STA", &a::STA, &a::IZX, 6},
                {"???", &a::NOP, &a::IMP, 2},
                {"???", &a::XXX, &a::IMP, 6},
                {"STY", &a::STY, &a::ZP0, 3},
                {"STA", &a::STA, &a::ZP0, 3},
                {"STX", &a::STX, &a::ZP0, 3},
                {"???", &a::XXX, &a::IMP, 3},
                {"DEY", &a::DEY, &a::IMP, 2},
                {"???", &a::NOP, &a::IMP, 2},
                {"TXA", &a::TXA, &a::IMP, 2},
                {"???", &a::XXX, &a::IMP, 2},
                {"STY", &a::STY, &a::ABS, 4},
                {"STA", &a::STA, &a::ABS, 4},
                {"STX", &a::STX, &a::ABS, 4},
                {"???", &a::XXX, &a::IMP, 4},
                {"BCC", &a::BCC, &a::REL, 2},
                {"STA", &a::STA, &a::IZY, 6},
                {"???", &a::XXX, &a::IMP, 2},
                {"???", &a::XXX, &a::IMP, 6},
                {"STY", &a::STY, &a::ZPX, 4},
                {"STA", &a::STA, &a::ZPX, 4},
                {"STX", &a::STX, &a::ZPY, 4},
                {"???", &a::XXX, &a::IMP, 4},
                {"TYA", &a::TYA, &a::IMP, 2},
                {"STA", &a::STA, &a::ABY, 5},
                {"TXS", &a::TXS, &a::IMP, 2},
                {"???", &a::XXX, &a::IMP, 5},
                {"???", &a::NOP, &a::IMP, 5},
                {"STA", &a::STA, &a::ABX, 5},
                {"???", &a::XXX, &a::IMP, 5},
                {"???", &a::XXX, &a::IMP, 5},
                {"LDY", &a::LDY, &a::IMM, 2},
                {"LDA", &a::LDA, &a::IZX, 6},
                {"LDX", &a::LDX, &a::IMM, 2},
                {"???", &a::XXX, &a::IMP, 6},
                {"LDY", &a::LDY, &a::ZP0, 3},
                {"LDA", &a::LDA, &a::ZP0, 3},
                {"LDX", &a::LDX, &a::ZP0, 3},
                {"???", &a::XXX, &a::IMP, 3},
                {"TAY", &a::TAY, &a::IMP, 2},
                {"LDA", &a::LDA, &a::IMM, 2},
                {"TAX", &a::TAX, &a::IMP, 2},
                {"???", &a::XXX, &a::IMP, 2},
                {"LDY", &a::LDY, &a::ABS, 4},
                {"LDA", &a::LDA, &a::ABS, 4},
                {"LDX", &a::LDX, &a::ABS, 4},
                {"???", &a::XXX, &a::IMP, 4},
                {"BCS", &a::BCS, &a::REL, 2},
                {"LDA", &a::LDA, &a::IZY, 5},
                {"???", &a::XXX, &a::IMP, 2},
                {"???", &a::XXX, &a::IMP, 5},
                {"LDY", &a::LDY, &a::ZPX, 4},
                {"LDA", &a::LDA, &a::ZPX, 4},
                {"LDX", &a::LDX, &a::ZPY, 4},
                {"???", &a::XXX, &a::IMP, 4},
                {"CLV", &a::CLV, &a::IMP, 2},
                {"LDA", &a::LDA, &a::ABY, 4},
                {"TSX", &a::TSX, &a::IMP, 2},
                {"???", &a::XXX, &a::IMP, 4},
                {"LDY", &a::LDY, &a::ABX, 4},
                {"LDA", &a::LDA, &a::ABX, 4},
                {"LDX", &a::LDX, &a::ABY, 4},
                {"???", &a::XXX, &a::IMP, 4},
                {"CPY", &a::CPY, &a::IMM, 2},
                {"CMP", &a::CMP, &a::IZX, 6},
                {"???", &a::NOP, &a::IMP, 2},
                {"???", &a::XXX, &a::IMP, 8},
                {"CPY", &a::CPY, &a::ZP0, 3},
                {"CMP", &a::CMP, &a::ZP0, 3},
                {"DEC", &a::DEC, &a::ZP0, 5},
                {"???", &a::XXX, &a::IMP, 5},
                {"INY", &a::INY, &a::IMP, 2},
                {"CMP", &a::CMP, &a::IMM, 2},
                {"DEX", &a::DEX, &a::IMP, 2},
                {"???", &a::XXX, &a::IMP, 2},
                {"CPY", &a::CPY, &a::ABS, 4},
                {"CMP", &a::CMP, &a::ABS, 4},
                {"DEC", &a::DEC, &a::ABS, 6},
                {"???", &a::XXX, &a::IMP, 6},
                {"BNE", &a::BNE, &a::REL, 2},
                {"CMP", &a::CMP, &a::IZY, 5},
                {"???", &a::XXX, &a::IMP, 2},
                {"???", &a::XXX, &a::IMP, 8},
                {"???", &a::NOP, &a::IMP, 4},
                {"CMP", &a::CMP, &a::ZPX, 4},
                {"DEC", &a::DEC, &a::ZPX, 6},
                {"???", &a::XXX, &a::IMP, 6},
                {"CLD", &a::CLD, &a::IMP, 2},
                {"CMP", &a::CMP, &a::ABY, 4},
                {"NOP", &a::NOP, &a::IMP, 2},
                {"???", &a::XXX, &a::IMP, 7},
                {"???", &a::NOP, &a::IMP, 4},
                {"CMP", &a::CMP, &a::ABX, 4},
                {"DEC", &a::DEC, &a::ABX, 7},
                {"???", &a::XXX, &a::IMP, 7},
                {"CPX", &a::CPX, &a::IMM, 2},
                {"SBC", &a::SBC, &a::IZX, 6},
                {"???", &a::NOP, &a::IMP, 2},
                {"???", &a::XXX, &a::IMP, 8},
                {"CPX", &a::CPX, &a::ZP0, 3},
                {"SBC", &a::SBC, &a::ZP0, 3},
                {"INC", &a::INC, &a::ZP0, 5},
                {"???", &a::XXX, &a::IMP, 5},
                {"INX", &a::INX, &a::IMP, 2},
                {"SBC", &a::SBC, &a::IMM, 2},
                {"NOP", &a::NOP, &a::IMP, 2},
                {"???", &a::SBC, &a::IMP, 2},
                {"CPX", &a::CPX, &a::ABS, 4},
                {"SBC", &a::SBC, &a::ABS, 4},
                {"INC", &a::INC, &a::ABS, 6},
                {"???", &a::XXX, &a::IMP, 6},
                {"BEQ", &a::BEQ, &a::REL, 2},
                {"SBC", &a::SBC, &a::IZY, 5},
                {"???", &a::XXX, &a::IMP, 2},
                {"???", &a::XXX, &a::IMP, 8},
                {"???", &a::NOP, &a::IMP, 4},
                {"SBC", &a::SBC, &a::ZPX, 4},
                {"INC", &a::INC, &a::ZPX, 6},
                {"???", &a::XXX, &a::IMP, 6},
                {"SED", &a::SED, &a::IMP, 2},
                {"SBC", &a::SBC, &a::ABY, 4},
                {"NOP", &a::NOP, &a::IMP, 2},
                {"???", &a::XXX, &a::IMP, 7},
                {"???", &a::NOP, &a::IMP, 4},
                {"SBC", &a::SBC, &a::ABX, 4},
                {"INC", &a::INC, &a::ABX, 7},
                {"???", &a::XXX, &a::IMP, 7},
        };
    }


    CPU::~CPU() {

    }


// Modes
    uint8_t CPU::ZP0() {
        addr_abs = readPc();
        return 0;
    }

// arg is the pc
// PEAK(arg) + x) & 0xff
    uint8_t CPU::ZPX() {
        addr_abs = ((readPc() + x) & 0xFF);
        return 0;
    }

    uint8_t CPU::ZPY() {
        addr_abs = ((readPc() + y) & 0xFF);
        return 0;
    }

    uint8_t CPU::ABS() {
        addr_abs = readPc() + (readPc() << 8);
        return 0;
    }

    uint8_t CPU::ABX() {
        auto low = readPc();
        auto high = readPc();
        addr_abs = low + (high << 8) + x;
        return (addr_abs & 0xff00) != (high << 8);
    }

    uint8_t CPU::ABY() {
        auto low = readPc();
        auto high = readPc();
        addr_abs = low + (high << 8) + y;
        return (addr_abs & 0xff00) != (high << 8);
    }

// Test Bits in Memory with Accumulator
// Flags Out: Z, N, V
    uint8_t CPU::BIT() {
        fetch();
        uint8_t temp = a & fetch();
        updateFlagZ(temp);
        updateFlagN(fetched);
        // why compare with 6?
        setFlag(V, fetched & (1 << 6));
        return 0;
    }

// This is no addition data required for instruction use this mode,
// the instruction do something very simple just like sets a status bit.
// However we will need target the accumulator, for instructions like PHA.
    uint8_t CPU::IMP() {
        fetched = a;
        return 0;
    }

/**
 * Uses the 8-bit operand itself as the value for the operation,
 * rather than fetching a value from a memory address.
 */
    uint8_t CPU::IMM() {
        addr_abs = pc++;
        return 0;
    }

    uint8_t CPU::REL() {
        // this is a signed offset.
        addr_rel = readPc();
        if (addr_rel & 0x80) {
            addr_rel |= 0xFF00;
        }
        return 0;
    }

// normally read_16(read_16(pc)), but that's a bug.
// This instruction is unusual in that it has a bug in the hardware! To emulate its
// function accurately, we also need to emulate this bug. If the low byte of the
// supplied address is 0xFF, then to cpuRead the high byte of the actual address
// we need to cross a page boundary. This doesnt actually work on the chip as
// designed, instead it wraps back around in the same page, yielding an
// invalid actual address
    uint8_t CPU::IND() {
        auto ptr = readPc16();

        if ((ptr & 0xff) == 0xff) { // bug is that we can't cross the page
            addr_abs = (read(ptr & 0xff) << 8) | read(ptr);
        } else {
            addr_abs = read16(ptr);
        }
        return 0;
    }

// arg = readPc()
// read16(arg + x)
    uint8_t CPU::IZX() {
        addr_abs = read16(readPc() + x);
        return 0;
    }

    uint8_t CPU::IZY() {
        auto base = read16(readPc());
        addr_abs = base + y;
        return (addr_abs & 0xff00) != (base & 0xff00);
    }

// Opcodes.
    uint8_t CPU::AND() {
        fetch();
        a = a & fetched;
        updateFlagZ(a);
        updateFlagN(a);
        return 1;
    }

// if is imp, shift to a
// else shift to memory inplace.
// Flags Out: N, Z, C
    uint8_t CPU::ASL() {
        fetch();
        uint16_t tmp = (uint16_t) fetched << 1;
        updateFlagN(tmp & 0xff);
        updateFlagZ(tmp & 0xff);
        updateFlagC(tmp);

        if (isImp()) {
            a = tmp & 0xFF;
        } else {
            write(addr_abs, tmp & 0xFF);
        }
        return 0;
    }

    uint8_t CPU::BCC() {
        return branchWithCondition([](CPU *cpu) {
            return !cpu->getFlag(C);
        });
    }

    uint8_t CPU::BCS() {
        return branchWithCondition([](CPU *cpu) {
            return !!cpu->getFlag(C);
        });
    }

    uint8_t CPU::BEQ() {
        return branchWithCondition([](CPU *cpu) {
            return !!cpu->getFlag(Z);
        });
    }

    uint8_t CPU::BNE() {
        return branchWithCondition([](CPU *cpu) {
            return !cpu->getFlag(Z);
        });
    }

// Branch on result minus
// Branch if negative
    uint8_t CPU::BMI() {
        return branchWithCondition([](CPU *cpu) {
            return !!cpu->getFlag(N);
        });
    }

// Branch on result Plus
// Branch if positive
    uint8_t CPU::BPL() {
        return branchWithCondition([](CPU *cpu) {
            return !cpu->getFlag(N);
        });
    }

    uint8_t CPU::BVC() {
        return branchWithCondition([](CPU *cpu) {
            return !cpu->getFlag(V);
        });
    }

    uint8_t CPU::BVS() {
        return branchWithCondition([](CPU *cpu) {
            return !!cpu->getFlag(V);
        });
    }

/**
 * A - M
 */
    uint8_t CPU::CMP() {
        compare(a);
        return 1;
    }

    uint8_t CPU::CPX() {
        compare(x);
        return 1;
    }

    uint8_t CPU::CPY() {
        compare(y);
        return 1;
    }

/**
 * Flags Out: Z, N
 */
    uint8_t CPU::DEC() {
        uint16_t value = fetch() - 1;
        write(addr_abs, value & 0xFF);
        updateFlagZ(value & 0xFF);
        updateFlagN(value & 0XFF);
        return 0;
    }

    uint8_t CPU::DEX() {
        x--;
        updateFlagZ(x);
        updateFlagN(x);
        return 0;
    }

    uint8_t CPU::DEY() {
        y--;
        updateFlagZ(y);
        updateFlagN(y);
        return 0;
    }

/**
 * A = A xor M
 * Flags Out: N, Z
 */
    uint8_t CPU::EOR() {
        fetch();
        a = a ^ fetched;
        updateFlagN(a);
        updateFlagZ(a);
        return 1;
    }

/**
 * M = M + 1
 * Flags Out: N, Z
 */
    uint8_t CPU::INC() {
        fetch();
        uint8_t tmp = fetch() + 1;
        write(addr_abs, tmp);
        updateFlagN(tmp);
        updateFlagZ(tmp);
        return 0;
    }

/**
 * X = X + 1
 * Flags Out: N, Z
 */
    uint8_t CPU::INX() {
        x++;
        updateFlagZ(x);
        updateFlagZ(x);
        return 0;
    }

/**
 * Y = Y + 1
 * Flags Out: N, Z
 */
    uint8_t CPU::INY() {
        y++;
        updateFlagZ(y);
        updateFlagZ(y);
        return 0;
    }

/**
 * unCondition jump
 */
    uint8_t CPU::JMP() {
        pc = addr_abs;
        return 0;
    }

/**
 * Jump to subroutine
 */
    uint8_t CPU::JSR() {
        push16(pc);
        pc = addr_abs;
        return 0;
    }

/**
 * A = M
 * Flags Out: N, Z
 */
    uint8_t CPU::LDA() {
        a = fetch();
        updateFlagZ(a);
        updateFlagN(a);
        return 1;
    }

/**
 * X = M
 * Flags Out: N, Z
 */
    uint8_t CPU::LDX() {
        x = fetch();
        updateFlagZ(x);
        updateFlagN(x);
        return 1;
    }

/**
 * Y = M
 * Flags Out: N, Z
 */
    uint8_t CPU::LDY() {
        y = fetch();
        updateFlagZ(y);
        updateFlagN(y);
        return 1;
    }

/**
 * shift right m and set to m or a
 * Flags Out: N, Z, C
 */
    uint8_t CPU::LSR() {
        fetch();
        // Still, strange C
        setFlag(C, fetched & 0x0001);
        uint16_t tmp = fetched >> 1;
        updateFlagZ(tmp);
        setFlag(N, tmp);
        if (isImp()) {
            a = tmp & 0xFF;
        } else {
            write(addr_abs, tmp & 0xFF);
        }
        return 0;
    }

    uint8_t CPU::NOP() {
        switch (opcode) {
            case 0x1C:
            case 0x3C:
            case 0x5C:
            case 0x7C:
            case 0xDC:
            case 0xFC:
                return 1;
        }
        return 0;
    }

/**
 * A = A | M
 * Flags Out: N, Z
 */
    uint8_t CPU::ORA() {
        fetch();
        a = a | fetched;
        updateFlagZ(a);
        updateFlagZ(a);
        return 1;
    }

/**
 * Push A
 */
    uint8_t CPU::PHA() {
        push(a);
        return 0;
    }

/**
 * push(s)
 * set B and U to 1 before push
 */
    uint8_t CPU::PHP() {
        push(status | U | B);
        setFlag(B, false);
        setFlag(U, false);
        return 0;
    }

/**
 * pop a
 * Flags Out: N, Z
 */
    uint8_t CPU::PLA() {
        a = pop();
        updateFlagZ(a);
        updateFlagN(a);
        return 0;
    }

/**
 * pop status
 * Need set U to 1
 */
    uint8_t CPU::PLP() {
        status = pop();
        setFlag(U, true);
        return 0;
    }

/**
 * v = M << 1 | C
 */
    uint8_t CPU::ROL() {
        uint16_t tmp = (uint16_t) (fetch() << 1) | getCFlag();
        setFlag(C, tmp & 0xFF00);
        updateFlagZ(tmp);
        updateFlagN(tmp);

        if (isImp()) {
            a = tmp & 0xFF;
        } else {
            write(addr_abs, tmp & 0xFF);
        }
        return 0;
    }


/**
 * v = (C << 7) | M >> 1
 */
    uint8_t CPU::ROR() {
        uint8_t tmp = (getCFlag() << 7) | (fetch() >> 1);
        setFlag(C, fetched & 0x1);
        updateFlagZ(tmp);
        updateFlagN(tmp);

        if (isImp()) {
            a = tmp & 0xFF;
        } else {
            write(addr_abs, tmp & 0xFF);
        }
        return 0;
    }

/**
 * pop pc
 * @return
 */
    uint8_t CPU::RTS() {
        pc = pop16();
        return 0;
    }

/**
 * set Carry Flag  1
 */
    uint8_t CPU::SEC() {
        setFlag(C, true);
        return 0;
    }

/**
 * Set D Flag 1
 * @return
 */
    uint8_t CPU::SED() {
        setFlag(D, true);
        return 0;
    }

    uint8_t CPU::SEI() {
        setFlag(I, true);
        return 0;
    }

/**
 * M = A
 * @return
 */
    uint8_t CPU::STA() {
        write(addr_abs, a);
        return 0;
    }

/**
 * M = X
 * @return
 */
    uint8_t CPU::STX() {
        write(addr_abs, x);
        return 0;
    }

/**
 * M = Y
 * @return
 */
    uint8_t CPU::STY() {
        write(addr_abs, y);
        return 0;
    }

/**
 * X = A
 * Flags Out: N, Z
 * @return
 */
    uint8_t CPU::TAX() {
        x = a;
        updateFlagN(x);
        updateFlagZ(x);
        return 0;
    }

/**
 * Y = A
 * Flags Out: N, Z
 * @return
 */
    uint8_t CPU::TAY() {
        y = a;
        updateFlagN(y);
        updateFlagZ(y);
        return 0;
    }

    uint8_t CPU::TSX() {
        x = sp;
        updateFlagN(x);
        updateFlagZ(x);
        return 0;
    }

    uint8_t CPU::TXA() {
        a = x;
        updateFlagN(a);
        updateFlagZ(a);
        return 0;
    }

    uint8_t CPU::TXS() {
        sp = x;
        updateFlagN(sp);
        updateFlagZ(sp);
        return 0;
    }

    uint8_t CPU::TYA() {
        a = y;
        updateFlagN(a);
        updateFlagN(a);
        return 0;
    }

// Force Break
    uint8_t CPU::BRK() {
        setFlag(I, true);

        pc++;
        push16(pc);

        setFlag(B, true);
        push(status);
        setFlag(B, false);

        pc = read16(0xFFFE);
        return 0;
    }

    uint8_t CPU::CLC() {
        setFlag(C, false);
        return 0;
    }

    uint8_t CPU::CLD() {
        setFlag(D, false);
        return 0;
    }

// clear interrupt disable flag
    uint8_t CPU::CLI() {
        setFlag(I, false);
        return 0;
    }

// clear overflow flag
    uint8_t CPU::CLV() {
        setFlag(V, false);
        return 0;
    }

// a = a + fetched.
    uint8_t CPU::ADC() {
        fetch();
        uint16_t tmp = (uint16_t) a + (uint16_t) fetched + (uint16_t) !!getCFlag();
        updateFlagC(tmp);
        updateFlagZ(tmp & 0xff);
        updateFlagN(tmp & 0x80);
        updateFlagV(a, fetched, tmp & 0xFF);
        a = tmp & 0xFF;
        return 1;
    }


// A = A - M - (1 - C)
// A = A - M - 1 + C
// A = A + (~M + 1) -1 + C
// A = A + ~M + C
// Out Flats: C Z N V
    uint8_t CPU::SBC() {
        fetch();
        uint8_t value = ((uint16_t) fetched) ^0xFF;
        uint16_t tmp = (uint16_t) a + (uint16_t) value + (uint16_t) getCFlag();
        updateFlagC(tmp);
        updateFlagZ(tmp & 0xff);
        updateFlagN(tmp & 0x80);
        updateFlagV(a, value, tmp & 0xFF);
        a = tmp & 0xFF;
        return 1;
    }


    uint8_t CPU::RTI() {
        // cpuRead the pc
        pc = pop16();

        // cpuRead the status
        status = pop16();
        // clear the B and U
        setFlag(B, false);
        setFlag(U, false);
        return 0;
    }

    uint8_t CPU::XXX() {
        return 0;
    }

    void CPU::reset() {
        a = 0;
        x = 0;
        y = 0;

        // look 0x01fd is the start sp address?
        sp = 0xFD;
        status = 0x00 | U;

        // the begin address is at 0xfffc
        pc = read16(0xFFFC);

        addr_rel = 0x00;
        addr_abs = 0x00;
        fetched = 0;
        cycles = 8;
    }

    void CPU::irq() {
        if (!getFlag(I)) {
            push16(pc);

            setFlag(B, false);
            setFlag(U, true);
            setFlag(I, true);

            push(status);

            pc = read16(0xFFFE);

            cycles = 7;
        }
    }

    void CPU::nmi() {
        push16(pc);

        setFlag(B, false);
        setFlag(U, true);
        setFlag(I, true);

        push(status);

        pc = read16(0xFFFA);

        cycles = 8;
    }

    uint8_t CPU::read(uint16_t addr) {
        return bus->read(addr, false);
    }

    void CPU::write(uint16_t addr, uint8_t data) {
        bus->write(addr, data);
    }

    void CPU::clock() {
        if (cycles == 0) {
            opcode = readPc();

            cycles = lookup[opcode].cycles;
            auto inst = this->lookup[opcode];
            auto overFlow = (this->*inst.addrmode)();
            auto careOverFlow = (this->*inst.operate)();

            cycles += (overFlow & careOverFlow);
        }
        cycles--;
    }

    uint8_t CPU::readPc() {
        return read(pc++);
    }

    uint16_t CPU::readPc16() {
        return readPc() | (readPc() << 8);
    }

    uint16_t CPU::read16(uint16_t addr) {
        // over flow
        if (addr == 0xFFFF) {
            throw std::out_of_range("read16 address overflow");
        }

        return read(addr) + (read(addr + 1) << 8);
    }

    uint8_t CPU::fetch() {
        // because IMP didn't use the addr_abs
        if (!isImp()) {
            fetched = read(addr_abs);
        }
        return fetched;
    }

    inline void CPU::updateFlagZ(uint8_t data) {
        setFlag(Z, data == 0);
    }

    inline void CPU::updateFlagN(uint8_t data) {
        setFlag(N, data & 0x80);
    }

    uint8_t CPU::branchWithCondition(bool (*condition)(CPU *)) {
        if (condition(this)) {
            cycles++;
            // 感觉这里addr_rel是负的也可以啊
            addr_abs = pc + addr_rel;
            if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
                cycles++;
            }
            pc = addr_abs;
        }
        return 0;
    }

    void CPU::updateFlagV(uint8_t a1, uint8_t a2, uint8_t sum) {
        // a, b same sign, a sum not same sign
        setFlag(V, ((~(a1 ^ a2)) & (a1 ^ sum)) & 0x80);
    }

    void CPU::updateFlagC(uint16_t data) {
        setFlag(C, data > 0xFF);
    }

    uint8_t CPU::getCFlag() {
        return getFlag(C);
    }


    void CPU::push(uint8_t data) {
        write(0x0100 + sp--, data);
    }

    void CPU::push16(uint16_t data) {
        push((data >> 8) & 0xff);
        push(data & 0xff);
    }

    uint8_t CPU::pop() {
        return read(0x0100 + ++sp);
    }

    uint16_t CPU::pop16() {
        return pop() << 8 | pop();
    }

    uint8_t CPU::getFlag(FLAGS f) const {
        return ((status & f) > 0) ? 1 : 0;
    }

    void CPU::setFlag(FLAGS f, bool v) {
        if (v) {
            status |= f;
        } else {
            status &= ~f;
        }
    }

    bool CPU::isImp() {
        return lookup[opcode].addrmode == &CPU::IMP;
    }

    void CPU::compare(uint8_t target) {
        fetch();
        // 0x81 - 0x00 = 0x81
        // what to set the negative??
        uint16_t tmp = (uint16_t) target - (uint16_t) fetched;
        setFlag(C, target >= fetched);
        updateFlagZ(tmp & 0xFF);
        updateFlagN(tmp & 0xff);
    }

    std::map<uint16_t, std::string> CPU::disassemble(uint16_t start, uint16_t end) {
        std::map<uint16_t, std::string> rst;
        uint32_t addr = start;

        auto hex = [](uint32_t n, uint8_t d) {
            std::string s(d, '0');
            for (int i = d - 1; i >= 0; i--, n >>= 4) {
                s[i] = "0123456789ABCDEF"[n & 0xF];
            }
            return s;
        };

        auto read8 = [&addr, this]() {
            return bus->read(addr++, true);
        };

        auto read16 = [&read8]() {
            auto lo = read8();
            auto hi = read8();
            return lo | hi << 8;
        };

        // 有可能overflow啊。
        while (addr <= end) {
            auto line_addr = addr;
            auto op = read8();
            std::string sInst = "$" + hex(line_addr, 4) + ": ";
            uint8_t value, lo;

            auto inst = lookup[op];

            sInst += inst.name + " ";

            if (inst.addrmode == &CPU::IMP) {
                sInst += " {IMP}";
            } else if (inst.addrmode == &CPU::IMM) {
                value = read8();
                sInst += "#$" + hex(value, 2) + " {IMM}";
            } else if (inst.addrmode == &CPU::ZP0) {
                sInst += "$" + hex(read8(), 2) + " {ZP0}";
            } else if (inst.addrmode == &CPU::ZPX) {
                lo = read8();
                sInst += "$" + hex(lo, 2) + ", X {ZPX}";
            } else if (inst.addrmode == &CPU::ZPY) {
                lo = read8();
                sInst += "$" + hex(lo, 2) + ", Y {ZPY}";
            } else if (inst.addrmode == &CPU::IZX) {
                sInst += "($" + hex(read8(), 2) + ", X) {IZX}";
            } else if (inst.addrmode == &CPU::IZY) {
                sInst += "($" + hex(read8(), 2) + "), Y {IZY}";
            } else if (inst.addrmode == &CPU::ABS) {
                sInst += "$" + hex(read16(), 4) + " {ABS}";
            } else if (inst.addrmode == &CPU::ABX) {
                sInst += "$" + hex(read16(), 4) + ", X {ABX}";
            } else if (inst.addrmode == &CPU::ABY) {
                sInst += "$" + hex(read16(), 4) + ", Y {ABY}";
            } else if (inst.addrmode == &CPU::IND) {
                sInst += "($" + hex(read16(), 4) + ") {IND}";
            } else if (inst.addrmode == &CPU::REL) {
                value = read8();
                sInst += "$" + hex(value, 2) + " [$" + hex(addr + (int8_t) value, 4) + "] {REL}";
            } else {
                throw std::runtime_error("There's some mode we forget");
            }

            rst[line_addr] = sInst;
        }

        return rst;
    }

    bool CPU::complete() const {
        return this->cycles == 0;
    }

    void CPU::print(uint16_t ip, int before, int after) {
        auto inst_map = disassemble(0x0, 0xFFFF);
    }

    void CPU::loadByteCodeInHex(const std::string &byteCodeInHex) {
        std::stringstream ss;
        uint16_t nOffset = 0x8000;
        ss << byteCodeInHex;
        while (!ss.eof()) {
            // yes, this will jump space.
            std::string b;
            ss >> b;
            bus->write(nOffset++, (uint8_t) std::stol(b, nullptr, 16));
        }
        bus->write(0xFFFC, 0x00);
        bus->write(0xFFFD, 0x80);
    }
}