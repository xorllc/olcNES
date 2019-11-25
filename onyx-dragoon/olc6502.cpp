#include <cstdint>
#include "olc6502.h"
#include "bus.h"

olc6502::olc6502(){}
olc6502::~olc6502(){}

uint8_t olc6502::read(uint16_t a)
{
    return bus->read(a, false);
}

void olc6502::write(uint16_t a, uint8_t d)
{
    bus->write(a, d);
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
