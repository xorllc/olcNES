#pragma once

class Bus;

class olc6502
{
    public:
        olc6502();
        ~olc6502();

        void ConnectBus(Bus *n) {bus = n;}

    private:
        Bus *bus = nullptr;
};
