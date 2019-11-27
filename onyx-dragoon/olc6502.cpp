#include <cstdint>
#include "olc6502.h"
#include "bus.h"

// Constructor
olc6502::olc6502()
{
    // Assembles the translation table. It's big, it's ugly, but it yields a convenient way
    // to emulate the 6502. I'm certain there are some "code-golf" strategies to reduce this
    // but I've deliberately kept it verbose for study and alteration

    // It is 16x16 entries. This gives 256 instructions. It is arranged to that the bottom
    // 4 bits of the instruction choose the column, and the top 4 bits choose the row.

    // For convenience to get function pointers to members of this class, I'm using this
    // or else it will be much much larger :D

    // The table is one big initialiser list of initialiser lists...
    //
    // The contents of each item in the initializer list is an initializer list, namely, a vector of instructions.
    // Each instruction has a human readable mneumonic, a fuction pointer to the instruction to execute, a function pointer
    // to the addressing mode, and the total number of clock cycles the instruction will take.
    using a = olc6502;
    lookup =
    {
        { "BRK", &a::BRK, &a::IMM, 7 },{ "ORA", &a::ORA, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::ZP0, 3 },{ "ASL", &a::ASL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHP", &a::PHP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::IMM, 2 },{ "ASL", &a::ASL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABS, 4 },{ "ASL", &a::ASL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
        { "BPL", &a::BPL, &a::REL, 2 },{ "ORA", &a::ORA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ZPX, 4 },{ "ASL", &a::ASL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLC", &a::CLC, &a::IMP, 2 },{ "ORA", &a::ORA, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABX, 4 },{ "ASL", &a::ASL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
        { "JSR", &a::JSR, &a::ABS, 6 },{ "AND", &a::AND, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "BIT", &a::BIT, &a::ZP0, 3 },{ "AND", &a::AND, &a::ZP0, 3 },{ "ROL", &a::ROL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLP", &a::PLP, &a::IMP, 4 },{ "AND", &a::AND, &a::IMM, 2 },{ "ROL", &a::ROL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "BIT", &a::BIT, &a::ABS, 4 },{ "AND", &a::AND, &a::ABS, 4 },{ "ROL", &a::ROL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
        { "BMI", &a::BMI, &a::REL, 2 },{ "AND", &a::AND, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ZPX, 4 },{ "ROL", &a::ROL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEC", &a::SEC, &a::IMP, 2 },{ "AND", &a::AND, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ABX, 4 },{ "ROL", &a::ROL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
        { "RTI", &a::RTI, &a::IMP, 6 },{ "EOR", &a::EOR, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "EOR", &a::EOR, &a::ZP0, 3 },{ "LSR", &a::LSR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHA", &a::PHA, &a::IMP, 3 },{ "EOR", &a::EOR, &a::IMM, 2 },{ "LSR", &a::LSR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::ABS, 3 },{ "EOR", &a::EOR, &a::ABS, 4 },{ "LSR", &a::LSR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
        { "BVC", &a::BVC, &a::REL, 2 },{ "EOR", &a::EOR, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ZPX, 4 },{ "LSR", &a::LSR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLI", &a::CLI, &a::IMP, 2 },{ "EOR", &a::EOR, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ABX, 4 },{ "LSR", &a::LSR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
        { "RTS", &a::RTS, &a::IMP, 6 },{ "ADC", &a::ADC, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ADC", &a::ADC, &a::ZP0, 3 },{ "ROR", &a::ROR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLA", &a::PLA, &a::IMP, 4 },{ "ADC", &a::ADC, &a::IMM, 2 },{ "ROR", &a::ROR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::IND, 5 },{ "ADC", &a::ADC, &a::ABS, 4 },{ "ROR", &a::ROR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
        { "BVS", &a::BVS, &a::REL, 2 },{ "ADC", &a::ADC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ZPX, 4 },{ "ROR", &a::ROR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEI", &a::SEI, &a::IMP, 2 },{ "ADC", &a::ADC, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ABX, 4 },{ "ROR", &a::ROR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
        { "???", &a::NOP, &a::IMP, 2 },{ "STA", &a::STA, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZP0, 3 },{ "STA", &a::STA, &a::ZP0, 3 },{ "STX", &a::STX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "DEY", &a::DEY, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 2 },{ "TXA", &a::TXA, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "STY", &a::STY, &a::ABS, 4 },{ "STA", &a::STA, &a::ABS, 4 },{ "STX", &a::STX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
        { "BCC", &a::BCC, &a::REL, 2 },{ "STA", &a::STA, &a::IZY, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZPX, 4 },{ "STA", &a::STA, &a::ZPX, 4 },{ "STX", &a::STX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "TYA", &a::TYA, &a::IMP, 2 },{ "STA", &a::STA, &a::ABY, 5 },{ "TXS", &a::TXS, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::NOP, &a::IMP, 5 },{ "STA", &a::STA, &a::ABX, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::XXX, &a::IMP, 5 },
        { "LDY", &a::LDY, &a::IMM, 2 },{ "LDA", &a::LDA, &a::IZX, 6 },{ "LDX", &a::LDX, &a::IMM, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "LDY", &a::LDY, &a::ZP0, 3 },{ "LDA", &a::LDA, &a::ZP0, 3 },{ "LDX", &a::LDX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "TAY", &a::TAY, &a::IMP, 2 },{ "LDA", &a::LDA, &a::IMM, 2 },{ "TAX", &a::TAX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "LDY", &a::LDY, &a::ABS, 4 },{ "LDA", &a::LDA, &a::ABS, 4 },{ "LDX", &a::LDX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
        { "BCS", &a::BCS, &a::REL, 2 },{ "LDA", &a::LDA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "LDY", &a::LDY, &a::ZPX, 4 },{ "LDA", &a::LDA, &a::ZPX, 4 },{ "LDX", &a::LDX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "CLV", &a::CLV, &a::IMP, 2 },{ "LDA", &a::LDA, &a::ABY, 4 },{ "TSX", &a::TSX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 4 },{ "LDY", &a::LDY, &a::ABX, 4 },{ "LDA", &a::LDA, &a::ABX, 4 },{ "LDX", &a::LDX, &a::ABY, 4 },{ "???", &a::XXX, &a::IMP, 4 },
        { "CPY", &a::CPY, &a::IMM, 2 },{ "CMP", &a::CMP, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPY", &a::CPY, &a::ZP0, 3 },{ "CMP", &a::CMP, &a::ZP0, 3 },{ "DEC", &a::DEC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INY", &a::INY, &a::IMP, 2 },{ "CMP", &a::CMP, &a::IMM, 2 },{ "DEX", &a::DEX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "CPY", &a::CPY, &a::ABS, 4 },{ "CMP", &a::CMP, &a::ABS, 4 },{ "DEC", &a::DEC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
        { "BNE", &a::BNE, &a::REL, 2 },{ "CMP", &a::CMP, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ZPX, 4 },{ "DEC", &a::DEC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLD", &a::CLD, &a::IMP, 2 },{ "CMP", &a::CMP, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ABX, 4 },{ "DEC", &a::DEC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
        { "CPX", &a::CPX, &a::IMM, 2 },{ "SBC", &a::SBC, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPX", &a::CPX, &a::ZP0, 3 },{ "SBC", &a::SBC, &a::ZP0, 3 },{ "INC", &a::INC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INX", &a::INX, &a::IMP, 2 },{ "SBC", &a::SBC, &a::IMM, 2 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::SBC, &a::IMP, 2 },{ "CPX", &a::CPX, &a::ABS, 4 },{ "SBC", &a::SBC, &a::ABS, 4 },{ "INC", &a::INC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
        { "BEQ", &a::BEQ, &a::REL, 2 },{ "SBC", &a::SBC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ZPX, 4 },{ "INC", &a::INC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SED", &a::SED, &a::IMP, 2 },{ "SBC", &a::SBC, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ABX, 4 },{ "INC", &a::INC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
    };
}

olc6502::~olc6502()
{
    // Destructor - has nothing to do
}

uint8_t olc6502::read(uint16_t a)
{
    return bus->read(a, false);
}

void olc6502::write(uint16_t a, uint8_t d)
{
    bus->write(a, d);
}

void olc6502::clock()
{
    // This emulator handles addressable data and configures the entire internal state
    // of the CPU in a single clock cycle, but still burns through remaining clock cycles
    // until the entire instruction is complete. In normal hardware, the entire instruction
    // is executed over several clock cycles, not like how it is here.
    if (cycles == 0)
    {
        // Read the opcode from the next instruction to execute.
        opcode = read(pc);

        // Increase the program counter to the next instruction to execute.
        pc++;

        // Get the number of cycles that this instruction will take.
        cycles = lookup[opcode].cycles;

        // Call the function referenced within the instruction struct
        // that configures the CPU registers based on the opcode's addressing
        // mode.
        // We want to save the result of this function in case we need to
        // increment the total number of cycles to burn through from the opcode
        // being handled.
        uint8_t additional_cycle1 = (this->*lookup[opcode].addrmode)();

        // Now that the addressing mode is configured, execute the instruction
        // that uses the data at the configured address.
        uint8_t additional_cycle2 = (this->*lookup[opcode].operate)();

        cycles += (additional_cycle1 & additional_cycle2);

    }

    cycles--;
}

uint8_t olc6502::GetFlag(FLAGS6502 f)
{
    return ((status & f) > 1) ? 1 : 0;
}

/*
 * Set the specified flag in the status register.
 */
void olc6502::SetFlag(FLAGS6502 f, bool v)
{
    /*
     * If we want to set the flag, we need to logical
     * OR its value with whatever data is in the status
     * register.
     */
    if (v)
        status |= f;

    /*
     * If we want to unset the flag, then we want to get
     * the status register, logical AND the status register
     * with "all bits except the register set" ("~f"), and
     * commit that to the status register.
     */
    else
        status &= ~f;
}


/*
 * Addressing modes
 */

/*
 * Implied addressing mode means no data is a part of the instruction, so,
 * configuring the address to act on doesn't matter.
 */
uint8_t olc6502::IMP()
{
    // The state of the CPU and addressable memory could change based on the
    // contents of the accumulator, so, we need to capture that.
    fetched = a;
    return 0;
}

/*
 * Immediate mode addressing means the data the CPU will use is provided as
 * a part of the instruction itself. Therefore, the program counter refers
 * to the data immediately after the opcode in memory.
 */
uint8_t olc6502::IMM()
{
    // addr_abs contains the instruction to execute when it's needed. Again,
    // this should not be conflated with the program counter itself.
    addr_abs = pc++;
    return 0;
}

/*
 * Implement zero page addressing. The address holding the data we're interested in
 * using as a part of the instruction operation's processing is at the first page
 * of addressable memory, the zero page. So, we want to read the data held in the
 * next byte referred to by the program counter and set the absolute address accordingly.
 */
uint8_t olc6502::ZP0()
{
    /*
     * Read the data referenced by the program counter.
     */
    addr_abs = read(pc);

    /*
     * Increment the program counter for future processing.
     */
    pc++;

    /*
     * Clear the high byte in the absolute address because this is zero page.
     */
    addr_abs &= 0x00FF;

    /*
     * This addressing mode cannot cause extra CPU cycles.
     */
    return 0;
}

                                     uint8_t olc6502::ZPX(){};
        uint8_t olc6502::ZPY(){};    uint8_t olc6502::REL(){};
        uint8_t olc6502::ABS(){};    uint8_t olc6502::ABX(){};
        uint8_t olc6502::ABY(){};    uint8_t olc6502::IND(){};
        uint8_t olc6502::IZX(){};    uint8_t olc6502::IZY(){};

        uint8_t olc6502::ADC(){};    uint8_t olc6502::AND(){};    uint8_t olc6502::ASL(){};    uint8_t olc6502::BCC(){};
        uint8_t olc6502::BCS(){};    uint8_t olc6502::BEQ(){};    uint8_t olc6502::BIT(){};    uint8_t olc6502::BMI(){};
        uint8_t olc6502::BNE(){};    uint8_t olc6502::BPL(){};    uint8_t olc6502::BRK(){};    uint8_t olc6502::BVC(){};
        uint8_t olc6502::BVS(){};    uint8_t olc6502::CLC(){};    uint8_t olc6502::CLD(){};    uint8_t olc6502::CLI(){};
        uint8_t olc6502::CLV(){};    uint8_t olc6502::CMP(){};    uint8_t olc6502::CPX(){};    uint8_t olc6502::CPY(){};
        uint8_t olc6502::DEC(){};    uint8_t olc6502::DEX(){};    uint8_t olc6502::DEY(){};    uint8_t olc6502::EOR(){};
        uint8_t olc6502::INC(){};    uint8_t olc6502::INX(){};    uint8_t olc6502::INY(){};    uint8_t olc6502::JMP(){};
        uint8_t olc6502::JSR(){};    uint8_t olc6502::LDA(){};    uint8_t olc6502::LDX(){};    uint8_t olc6502::LDY(){};
        uint8_t olc6502::LSR(){};    uint8_t olc6502::NOP(){};    uint8_t olc6502::ORA(){};    uint8_t olc6502::PHA(){};
        uint8_t olc6502::PHP(){};    uint8_t olc6502::PLA(){};    uint8_t olc6502::PLP(){};    uint8_t olc6502::ROL(){};
        uint8_t olc6502::ROR(){};    uint8_t olc6502::RTI(){};    uint8_t olc6502::RTS(){};    uint8_t olc6502::SBC(){};
        uint8_t olc6502::SEC(){};    uint8_t olc6502::SED(){};    uint8_t olc6502::SEI(){};    uint8_t olc6502::STA(){};
        uint8_t olc6502::STX(){};    uint8_t olc6502::STY(){};    uint8_t olc6502::TAX(){};    uint8_t olc6502::TAY(){};
        uint8_t olc6502::TSX(){};    uint8_t olc6502::TXA(){};    uint8_t olc6502::TXS(){};    uint8_t olc6502::TYA(){};

        uint8_t olc6502::XXX(){};
