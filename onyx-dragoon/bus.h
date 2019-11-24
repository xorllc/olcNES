#pragma once

#include <cstdint>
#include <array>
//#include "olc6502.h"

class Bus
{
    public:
        Bus();
        ~Bus();

    public:
        //olc6502 cpu;
        std::array<uint8_t, 64 * 2^10> ram;

    public:
        void write(uint16_t addr, uint8_t data);
        uint8_t read(uint16_t addr, bool bReadOnly = false);
};
