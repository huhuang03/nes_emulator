//
// Created by huhua on 2021/5/22.
//

#ifndef NES_CPU_H
#define NES_CPU_H

#include <cstdint>
#include <string>
#include <vector>
#include <map>

class Bus;

// 6502 cpu
class CPU {
public:
    CPU();
    ~CPU();

    bool complete() const;

public:
    enum FLAGS
    {
        C = (1 << 0),   // Carry Bit
        Z = (1 << 1),   // Zero
        I = (1 << 2),   // Disable Interrupts
        D = (1 << 3),   // Decimal Mode(unused in this impl)
        B = (1 << 4),   // Break
        U = (1 << 5),   // Unused
        V = (1 << 6),   // Overflow
        N = (1 << 7),   // Negative
    };

    uint8_t a = 0x00;   // Accumulator Reg
    uint8_t x = 0x00;   // x Reg
    uint8_t y = 0x00;   // Y Reg
    uint8_t sp = 0x0000;  // Stack Pointer
    uint16_t pc = 0x0000;  // Program Counter
    uint8_t status = 0x00;  // Status Reg

    // Address Mode
    // http://wiki.nesdev.com/w/index.php/CPU_addressing_modes
    uint8_t IMP();	uint8_t IMM();
    // PEEK(arg)
    uint8_t ZP0();	uint8_t ZPX();
    uint8_t ZPY();	uint8_t REL();
    uint8_t ABS();	uint8_t ABX();
    uint8_t ABY();	uint8_t IND();
    uint8_t IZX();	uint8_t IZY();


    // OpCodes
    // http://wiki.nesdev.com/w/index.php/CPU_unofficial_opcodes
    // doc/rockwell_r650x_r651x.pdf

    uint8_t ADC();	uint8_t AND();	uint8_t ASL();	uint8_t BCC();
    uint8_t BCS();	uint8_t BEQ();	uint8_t BIT();	uint8_t BMI();
    uint8_t BNE();	uint8_t BPL();	uint8_t BRK();	uint8_t BVC();
    uint8_t BVS();	uint8_t CLC();	uint8_t CLD();	uint8_t CLI();
    uint8_t CLV();	uint8_t CMP();	uint8_t CPX();	uint8_t CPY();
    uint8_t DEC();	uint8_t DEX();	uint8_t DEY();	uint8_t EOR();
    uint8_t INC();	uint8_t INX();	uint8_t INY();	uint8_t JMP();
    uint8_t JSR();	uint8_t LDA();	uint8_t LDX();	uint8_t LDY();
    uint8_t LSR();	uint8_t NOP();	uint8_t ORA();	uint8_t PHA();
    uint8_t PHP();	uint8_t PLA();	uint8_t PLP();	uint8_t ROL();
    uint8_t ROR();	uint8_t RTI();	uint8_t RTS();	uint8_t SBC();
    uint8_t SEC();	uint8_t SED();	uint8_t SEI();	uint8_t STA();
    uint8_t STX();	uint8_t STY();	uint8_t TAX();	uint8_t TAY();
    uint8_t TSX();	uint8_t TXA();	uint8_t TXS();	uint8_t TYA();
    // Present all illegal op codes.
    uint8_t XXX();

    void clock();
    void reset();
    // interruptRequest
    void irq();
    // nonMaskableInterruptRequest
    void nmi();

    uint8_t fetch();
    uint8_t fetched = 0x00;

    uint16_t addr_abs = 0x0000;
    uint16_t addr_rel = 0x00;
    uint8_t opcode = 0x00;
    uint8_t cycles = 0;

public:
    void print(uint16_t ip, int before, int after);
    void connectBus(Bus *n) { bus = n; };
    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t data);

    /**
     * @return key is instruction start pos, value is the instruction string.
     */
    std::map<uint16_t, std::string> disassemble(uint16_t start, uint16_t end);

    uint8_t getFlag(FLAGS f) const;
    void setFlag(FLAGS f, bool v);

    /**
     * Only load at 0x8000
     * @param byteCodeInHex
     */
    void loadByteCodeInHex(const std::string &byteCodeInHex);

private:
    Bus *bus;

    struct INSTRUCTION
    {
        std::string name;
        uint8_t (CPU::*operate)() = nullptr;
        uint8_t (CPU::*addrmode)() = nullptr;
        uint8_t cycles = 0;
    };

    std::vector<INSTRUCTION> lookup;

// helper functions
private:

    uint8_t readPc();

    /**
     * @return readPc() | (readPc << 8)
     */
    uint16_t readPc16();

    uint16_t read16(uint16_t addr);

    void updateFlagC(uint16_t data);
    void updateFlagZ(uint8_t data);
    void updateFlagN(uint8_t data);
    void updateFlagV(uint8_t a, uint8_t b, uint8_t sum);
    uint8_t getCFlag();

    void push(uint8_t);
    void push16(uint16_t);

    /**
     * Imp is a mode which take no address.
     */
    bool isImp();

    uint8_t pop();
    uint16_t pop16();

    /**
     * common login for compare inst
     */
    void compare(uint8_t target);

    uint8_t branchWithCondition(bool (*)(CPU *cpu));
};


#endif //NES_CPU_H
