#include "olc6502.h"

/*
 * The cstdint library takes standard data types like int, short, etc.
 * and renames them into explicit types like uint32_t. The width
 * of variables is more explicit here.
 *
*/
#include <cstdint>

#include "bus.h"

olc6502::olc6502()
{
}

olc6502::~olc6502()
{
}

uint8_t olc6502::read(uint16_t a)
{
    return bus->read(a, false);
}

void olc6502::write(uint16_t a, uint8_t d)
{
    bus->write(a, d);
}
