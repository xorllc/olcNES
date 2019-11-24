#pragma once

/*
 * The cstdint library takes standard data types like int, short, etc.
 * and renames them into explicit types like uint32_t. The width
 * of variables is more explicit here.
 *
*/
#include <cstdint>

class Bus;

class olc6502
{
    public:
        /*
         * Function prototypes for constructor and destructor.
         */
        olc6502();
        ~olc6502();

        /*
         * Function prototype for connecting the CPU to the bus.
         */
        void ConnectBus(Bus *n) { bus = n; }

    public:
        /*
         *
         */
        enum FLAGS6502
        {
            C = (1 << 0),  // Carry bit.
            Z = (1 << 1),  // Zero flag.
            I = (1 << 2),  // Zero flag.
            D = (1 << 3),  // Decimal mode.
            B = (1 << 4),  // Break
            U = (1 << 5),  // Unused
            V = (1 << 6),  // Overflow
            N = (1 << 7),  // Negative
        };

    private:
        Bus *bus = nullptr;

        /*
         * Function prototype for reading data from the CPU.
         */
        uint8_t read(uint16_t a);

        /*
         * Function prototype for writing data to the CPU.
         */
        void write(uint16_t a, uint8_t d);

};
