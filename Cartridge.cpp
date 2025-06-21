#include "Cartridge.h"
// #include<cstdint>
// #include<array>
// #include<string>
// #include<fstream>
// #include<iostream>
// #include<memory>

Cartridge::Cartridge(const std::string& sFileName) {
    // iNES header
    struct sHeader {
        char name[4];
        uint8_t prg_rom_chunks;
        uint8_t chr_rom_chunks;
        uint8_t mapper1;
        uint8_t mapper2;
        uint8_t prg_ram_size;
        uint8_t tv_system1;
        uint8_t tv_system2;
        char unused[5];
    } header;

    bImageValid = false;

    // to open the file:
    std::ifstream ifs;
    ifs.open(sFileName, std::ifstream::binary);

    // to read the header:
    if (ifs.is_open()) {
        ifs.read((char*)&header, sizeof(sHeader));

        // the next 512 bytes are used for training the information
        if (header.mapper1 & 0x04) {
            ifs.seekg(512, std::ios_base::cur);
        }

        // determine which mapper the rom is using
        nMapperID = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);
        mirror = (header.mapper1 & 0x01) ? VERTICAL : HORIZONTAL;

        // determining the file format - there are 3 types of iNES files
        uint8_t nFileType = 1;
        if (nFileType == 0) {

        }
        if (nFileType == 1) {
            // read how many banks of memory there are for the program memory
            // resize vector to that size
            // read data from file into the vector
            // a single bank of program memory is  16KB
            nPRGBanks = header.prg_rom_chunks; 
            vPRGMemory.resize(nPRGBanks * 16384); 
            ifs.read((char*)vPRGMemory.data(), vPRGMemory.size()); 

            // a single bank of character memory is 8KB
            // do the same for character memory
            nCHRBanks = header.chr_rom_chunks;
            if (nCHRBanks == 0) {
                vCHRMemory.resize(8192);
            }
            else vCHRMemory.resize(nCHRBanks * 8192);
            ifs.read((char*)vCHRMemory.data(), vCHRMemory.size());
        }
        if (nFileType == 2) {

        }

        // Load appropriate mapper
        // polymorphism to selectively which mapper class we want to use
        switch (nMapperID) {
            case 0:
                pMapper = std::make_shared<Mapper_000>(nPRGBanks, nCHRBanks);
                break;
        }
        
        bImageValid = true;
        ifs.close();
    }
}

Cartridge::~Cartridge() {

}

// Read and write fncs return boolean values to tell the
// calling system whether the cartridge system is handling
// the read or write. 

bool Cartridge::ImageValid()
{
	return bImageValid;
}

bool Cartridge::cpuRead(uint16_t addr, uint8_t& data) {
    uint32_t mapped_addr = 0;
    if (pMapper->cpuMapRead(addr, mapped_addr)) {
        data = vPRGMemory[mapped_addr];
        return true;
    }
    else return false;
}

bool Cartridge::cpuWrite(uint16_t addr, uint8_t data) {
    uint32_t mapped_addr = 0;
    if (pMapper->cpuMapWrite(addr, mapped_addr, data)) {
        vPRGMemory[mapped_addr] = data;
        return true;
    }
    else return false;
}

bool Cartridge::ppuRead (uint16_t addr, uint8_t& data) {
    uint32_t mapped_addr =0;
    if (pMapper->ppuMapRead(addr, mapped_addr)) {
        data = vCHRMemory[mapped_addr];
        return true;
    }
    else return false;
}

bool Cartridge::ppuWrite (uint16_t addr, uint8_t data) {
    uint32_t mapped_addr = 0;
    if (pMapper->ppuMapRead(addr, mapped_addr)) {
        vCHRMemory[mapped_addr] = data;
        return true;
    }
    else return false;
}

void Cartridge::reset() {
    // This does not reset ROM contents but it does reset the mapper
    if (pMapper != nullptr) {
        pMapper->reset();
    }
}