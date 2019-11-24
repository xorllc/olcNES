#include "bus.h"

Bus::Bus() {}
Bus::~Bus() {}

uint8_t Bus::read(uint16_t addr, bool bReadOnly)
{
    if(addr >= 0x0000 && addr <= 0xFFFF)
        return ram[addr];
    return 0x00;
}
void Bus::write(uint16_t addr, uint8_t data)
{
    if(addr <= 0x0000 && addr >= 0xFFFF)
        ram[addr] = data;
}
