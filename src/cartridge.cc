//
// Created by hwf on 2021/6/4.
//

#include <fstream>
#include "cartridge.h"
#include <stdexcept>
#include <iostream>
#include <SDKDDKVer.h>
#include <Windows.h>
//#include <unistd>

uint32_t Cartridge::UNIT_CHR = 8 * 1024;
uint32_t Cartridge::UNIT_PRG = 16 * 1024;

//#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
//#else
//#include <unistd.h>
//#define GetCurrentDir getcwd
//#endif

std::string get_current_dir() {
    char buff[FILENAME_MAX]; //create string buffer to hold path
    GetCurrentDir( buff, FILENAME_MAX );
    std::string current_working_dir(buff);
    return current_working_dir;
}

Cartridge::Cartridge(const std::string &sFileName) {
    std::cout << get_current_dir() << std::endl;
//    TCHAR pwd[MAX_PATH];
//    GetCurrentDirectory(MAX_PATH,pwd);
//    std::cout << pwd << std::endl;
//    std::string cwd = _getcwd();
//    std::filesystem::current_path();
//    char * cwd;
//    cwd = (char*) malloc( FILENAME_MAX * sizeof(char) );
//    getcwd(cwd, FILENAME_MAX);

    // http://wiki.nesdev.com/w/index.php/INES
    struct Header {
        // Constant $4E $45 $53 $1A ("NES" followed by MS-DOS end-of-file)
        char name[4];
        uint8_t prg_rom_chunks;
        uint8_t chr_rom_chunks;
        uint8_t mapper1;
        uint8_t mapper2;
        uint8_t prg_ram_size;
        uint8_t tv_system1;
        uint8_t tv_system2;
        char unused[5];
    } header{};

    std::ifstream ifs;
    ifs.open(sFileName, std::ifstream::binary);

    if (ifs.is_open()) {
        ifs.read((char*)&header, sizeof(Header));

        bool presentTrainer = header.mapper1 & 0b00001000;
        if (presentTrainer) {
            ifs.seekg(512, std::ios_base::cur);
        }

        nMapperID = (header.mapper1 >> 4) | (header.mapper1 >> 4 << 4);

        // ok now we read our prg and chr data
        uint32_t prg_data_size = header.prg_rom_chunks * UNIT_PRG;
        uint32_t chr_data_size = header.chr_rom_chunks * UNIT_CHR;

        vPRGMemory.resize(prg_data_size);
        ifs.read(reinterpret_cast<char *>(vPRGMemory.data()), prg_data_size);
        vCHRMemory.resize(chr_data_size);
        ifs.read(reinterpret_cast<char *>(vCHRMemory.data()), chr_data_size);

        switch (nMapperID) {
            case 0:
                pMapper = std::make_shared<Mapper000>(nPRGBanks, nCHRBanks);
                break;
            default:
                throw std::exception("For now, we only support mapper id 0");
        }
    } else {
        std::cout << "Can't read from cartridge " + sFileName << std::endl;
        throw std::invalid_argument("Can't read from cartridge " + sFileName);
    }

    ifs.close();

}

bool Cartridge::cpuRead(uint16_t addr, uint8_t &data) {
    uint32_t mapped_addr = 0;
    if (pMapper->cpuMapRead(addr, mapped_addr)) {
        data = vPRGMemory[mapped_addr];
        return true;
    }
    return false;
}

bool Cartridge::cpuWrite(uint16_t addr, uint8_t data) {
    return false;
}

bool Cartridge::ppuRead(uint16_t addr, uint8_t &data) {
    uint32_t mapped_addr = 0;
    if (pMapper->ppuMapRead(addr, mapped_addr)) {
        data = vCHRMemory[mapped_addr];
        return true;
    }
    return false;
}

bool Cartridge::ppuWrite(uint16_t addr, uint8_t data) {
    return false;
}
