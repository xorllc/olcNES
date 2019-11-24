
/*
 * Constructor, destructor, function prototypes,
 * and instance members for the bus.
 */
#include "bus.h"

/*
 * Implement the constructor for the bus.
 */
Bus::Bus()
{
    /*
     * Initialize the RAM by iterating over each address
     * in a for-each style, one byte at a time (since
     * each member of the array is uint8_t.
     */
    for (auto &i : ram) i = 0x00;

    cpu.ConnectBus(this);
}

/*
 * Implement the destructor for the bus.
 */
Bus::~Bus()
{
}

/*
 * Implement the write method.
 */
void Bus::write(uint16_t addr, uint8_t data)
{
    /*
     * This is currently the only device accessible on
     * the bus, and the only device that is writable for
     * that matter.
     *
     * Only write to RAM if the address is within the
     * valid range supported by the 6502.
     */
    if(addr >= 0x0000 && addr <= 0xFFFF)
        ram[addr] = data;
}

/*
 * Implement the read method.
 */
uint8_t Bus::read(uint16_t addr, bool bReadOnly)
{
    /*
     * This is currently the only device accessible on
     * the bus, and the only device that is readable for
     * that matter.
     *
     * Only read from RAM if the address is within the
     * valid range supported by the 6502.
     */
    if(addr >= 0x0000 && addr <= 0xFFFF)
        return ram[addr];

    return 0x00;
}
