#include "Bus.h"

Bus::Bus() {
    // connect CPU to communication bus
    cpu.ConnectBus(this);
}

Bus::~Bus() {

}

void Bus::SetSampleFrequency(uint32_t sample_rate) {
    dAudioTimePerSystemSample = 1.0 / (double)sample_rate;
    dAudioTimePerNESClock = 1.0 / 5369318.0; // PPU clock frequency
}

void Bus::cpuWrite(uint16_t addr, uint8_t data) {

    if (cart->cpuWrite(addr, data)) {

    }
    else if (addr >= 0x0000 && addr <= 0x1FFF) { // when cpu is targeted
        cpuRam[addr & 0x07FF] = data;
    }
    else if (addr >= 0x2000 && addr <= 0x3FFF) { // when ppu is targeted
        ppu.cpuWrite(addr & 0x0007, data);
    }
    else if ((addr >= 0x4000 && addr <= 0x4013) || addr == 0x4015 || addr == 0x4017) { // nes apu
        apu.cpuWrite(addr, data);
    }
    else if (addr == 0x4014) {
        dma_page = data;
        dma_addr = 0x00;
        dma_transfer = true;
    }
    else if (addr >= 0x4016 && addr <= 0x4017) {
        controller_state[addr & 0x0001] = controller[addr & 0x0001];
    }
    
}

uint8_t Bus::cpuRead(uint16_t addr, bool bReadOnly) {

    uint8_t data = 0x00;

    if (cart->cpuRead(addr, data)) {
        
    }
    else if (addr >= 0x0000 && addr <= 0x1FFF) { //  when cpu is targeted
        data = cpuRam[addr & 0x07FF]; // 
    }
    else if (addr == 0x4015) {
        data = apu.cpuRead(addr);
    }
    else if (addr >= 0x2000 && addr <= 0x3FFF) { // whne ppu is targeted
        data = ppu.cpuRead(addr & 0x0007, bReadOnly); // this logic bitwise AND makes sure the address stays in the address range. else it could be an invalid address type
    }
    else if (addr >= 0x4016 && addr <= 0x4017) {
        data = (controller_state[addr & 0x0001] & 0x80) > 0; // to extract msb
        controller_state[addr & 0x0001] <<= 1;
    }

    return data;
}

void Bus::insertCartridge(const std::shared_ptr<Cartridge>& cartridge) {
    this->cart = cartridge;
    ppu.ConnectCartridge(cartridge);
}

void Bus::reset() {
    cart->reset();
    cpu.reset();
    ppu.reset();
    nSystemClockCounter = 0;
    dma_page = 0x00;
    dma_addr = 0x00;
    dma_data = 0x00;
    dma_dummy = true;
    dma_transfer = true;
}

bool Bus::clock() {

    ppu.clock();
    apu.clock(); // also clock the apu

    if(nSystemClockCounter % 3 == 0) {
        if (dma_transfer) {
            if (dma_dummy) {
                if (nSystemClockCounter % 2 == 1) {
                    dma_dummy = false;
                }
            }
            else {
                if (nSystemClockCounter % 2 == 0) {
                    dma_data = cpuRead(dma_page << 8 | dma_addr);
                }
                else {
                    ppu.pOAM[dma_addr] = dma_data;
                    dma_addr++;

                    if (dma_addr == 0x00) {
                        dma_transfer = false;
                        dma_dummy = true;
                    }
                }
            }
        }
        else {
            cpu.clock();
        }
    }

    // Synchronizing with audio
    bool bAudioSampleReady = false;
    dAudioTime += dAudioTimePerNESClock;
    if (dAudioTime >= dAudioTimePerSystemSample) {
        dAudioTime -= dAudioTimePerSystemSample;
        dAudioSample = apu.GetOutputSample();
        bAudioSampleReady = true;
    }

    // Why are we clocking the NES per sound sample? That would mean we're calling the sound out function is 44k times per sec.
    // What actually happens:
    // Even though the pixel sound engine presents this functions requesting single sample at a time, the sample isn't actually requested in real audio time.
    // The sound hardware requests a block of 512 samples at atime and we see those being successively called by the function. So we're always emulating faster
    // than real time. We're emulating the audio faster than real time.

    if (ppu.nmi) {
        ppu.nmi = false;
        cpu.nmi();
    }

    nSystemClockCounter++;
    return bAudioSampleReady;
}