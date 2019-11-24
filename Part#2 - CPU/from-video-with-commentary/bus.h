#pragma once

/*
 * The cstdint library takes standard data types like int, short, etc.
 * and renames them into explicit types like uint32_t. The width
 * of variables is more explicit here.
 *
*/
#include <cstdint>

/*
 * Needed for the RAM that the bus reads data from and writes data to.
 */
#include <array>

/*
 * Implements the CPU attached to this bus.
 */
#include "olc6502.h"

class Bus
{
    public:
        Bus();
        ~Bus();

    public:
        /*
         * These are the devices that are connected to the bus, that is,
         * the instance members.
         */

        /*
         * The primary device that's connected to the bus is the CPU,
         * which is implemented in another module.
         */
        olc6502 cpu;

        /*
         *
         * Another device is RAM itself, which is the sole thing that
         * the CPU interacts with at this stage of the implementation.
         * This is 64 KiB of RAM...which is a large amount of memory
         * to be placed on the stack.
         */
        std::array<uint8_t, 64 * 2^10> ram;

    public:
        /*
         * These are the function prototypes of the bus instance.
         *
         * We know the bus is read from, and written to, by the CPU.
         * These are the function prototypes that the Bus instance will
         * implement in order to read and write data to RAM. RAM is the
         * initial resource the CPU targets through the bus.
         * 
         * The 6502 operates on data chunks from the bus one byte at a
         * time, hence the uint8_t for the data. However, the address of
         * the 6502 is 16 bytes.
         */
        void write(uint16_t addr, uint8_t data);
        uint8_t read(uint16_t addr, bool bReadOnly = false);
};
