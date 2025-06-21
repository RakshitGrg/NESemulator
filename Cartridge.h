#pragma once
#include<vector>
#include<cstdint>
#include<string>
#include<fstream>
#include<memory>

#include "Mapper_000.h"

class Cartridge {
    public: 
        Cartridge(const std::string& sFileName);
        ~Cartridge();

        enum MIRROR {
            HORIZONTAL,
            VERTICAL,
            ONESCREEN_LO,
            ONESCREEN_HI,
        } mirror = HORIZONTAL;

        bool ImageValid();

    private:
        bool bImageValid = false;
    // adding memory to the cartridge
        std::vector<uint8_t> vPRGMemory;
        std::vector<uint8_t> vCHRMemory;

        uint8_t nMapperID = 0; // tells which mapper we're using
        uint8_t nPRGBanks = 0; // stores how many banks of the resp memory there are
        uint8_t nCHRBanks = 0;

        std::shared_ptr<Mapper> pMapper;

    public:
        // communicate with main bus
        bool cpuRead(uint16_t addr, uint8_t &data);
        bool cpuWrite(uint16_t addr, uint8_t data);

        // communication with ppu bus
        bool ppuRead(uint16_t addr, uint8_t &data);
        bool ppuWrite(uint16_t addr, uint8_t data);

        void reset();
  
};