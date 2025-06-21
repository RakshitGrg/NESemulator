#include "Mapper_000.h"
// #include<cstdio>

Mapper_000::Mapper_000(uint8_t prgBanks, uint8_t chrBanks) : Mapper(prgBanks, chrBanks) {

}

Mapper_000::~Mapper_000() {

}

void Mapper_000::reset() {

}

bool Mapper_000::cpuMapRead(uint16_t addr, uint32_t &mapped_addr) {
    if (addr >= 0x8000 && addr <= 0xFFFF) {
        mapped_addr = addr & (nPRGBanks > 1 ? 0x7FFF : 0x3FFF);
        return true;
    }
    
    return false;
}

bool Mapper_000::cpuMapWrite(uint16_t addr, uint32_t &mapped_addr, uint8_t data) {
    if (addr >= 0x8000 && addr <= 0xFFFF) {
        mapped_addr = addr & (nPRGBanks > 1 ? 0x7FFF : 0x3FFF);
        return true;
    }
    return false;
}


// Note the difference in address ranges of the CPU and PPU!!
bool Mapper_000::ppuMapRead (uint16_t addr, uint32_t &mapped_addr) {
    if (addr >= 0x0000 && addr <= 0x1FFF) {
        mapped_addr = addr;
        return true;
    }

    return false;
}

bool Mapper_000::ppuMapWrite(uint16_t addr, uint32_t &mapped_addr) {
    if (addr >= 0x0000 && addr <= 0x1FFF) {
        if (nCHRBanks == 0) {
            mapped_addr = addr;
            return true;
        }
    }
    return false;
}

// We're using Object Oriented Programming to encapsulate functionality.

// Why we use the separate mapper class?
// It allows us to have additional variable rules and variables
// specific to this mapper implementation.