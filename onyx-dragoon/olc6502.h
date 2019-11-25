#pragma once

class Bus;

class olc6502
{
    public:
        olc6502();
        ~olc6502();

    public:

        uint8_t status = 0x00;  // Status Register. Accessible through a getter/setter and holds values
                                // from the enumeration above.

        void ConnectBus(Bus *n) {bus = n;}

        // The status register stores 8 flags. Ive enumerated these here for ease
        // of access. You can access the status register directly since its public.
        // The bits have different interpretations depending upon the context and
        // instruction being executed.
        enum FLAGS6502
        {
            C = (1 << 0),    // Carry Bit
            Z = (1 << 1),    // Zero
            I = (1 << 2),    // Disable Interrupts
            D = (1 << 3),    // Decimal Mode (unused in this implementation)
            B = (1 << 4),    // Break
            U = (1 << 5),    // Unused
            V = (1 << 6),    // Overflow
            N = (1 << 7),    // Negative
        };

    private:

        // Convenience functions to access the status register.
        // Wraps up the bitwise operations to set the values of the status register
        // depending on the flag we're interested in.
        uint8_t GetFlag(FLAGS6502 f);
        void    SetFlag(FLAGS6502 f, bool v);

        Bus *bus = nullptr;
        uint8_t read(uint16_t a);
        void write(uint16_t a, uint8_t d);
};
