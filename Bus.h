#pragma once
#include<cstdint>
#include<array>

#include "olc6502.h"
#include "olc2C02.h"
#include "olc2A03.h"
#include "Cartridge.h"

class Bus {
    public:
        Bus();
        ~Bus();

    // devices on the bus
        olc6502 cpu;
        olc2C02 ppu;
        olc2A03 apu;
        std::shared_ptr<Cartridge> cart;
        uint8_t cpuRam[2048]; // 64 * 1024
        uint8_t controller[2];

    // bus read and cpuWrite functions
        void cpuWrite(uint16_t addr, uint8_t data);
        uint8_t cpuRead (uint16_t addr, bool bReadOnly = false);

    // to interrogate the instance of the bus at any point of time to ask what the current output is
        double dAudioSample = 0.0;
        void SetSampleFrequency(uint32_t sample_rate); // to inform the apu about the temporal properites of the surrounding emulation system
    

    // the NES has three ways of interacting with the bus
        void insertCartridge(const std::shared_ptr<Cartridge>& cartridge); // takes shared ptr to a catridge object
        void reset();
        bool clock(); // to create one system tick of the emulation

    private:
        double dAudioTimePerSystemSample = 0.0f;
        double dAudioTimePerNESClock = 0.0;
        double dAudioTime = 0.0;

    private:
    // to count how many time the clock function has been called
        uint32_t nSystemClockCounter = 0;
        uint8_t controller_state[2];

        uint8_t dma_page = 0x00; // forms 16 bit address with dma_addr
        uint8_t dma_addr = 0x00;
        uint8_t dma_data = 0x00; // represents the byte of data in transit from the cpu's memory to the oam

        bool dma_transfer = false;
        bool dma_dummy = true;
};