# NES Emulator

## Description

This repository contains an NES emulator written in C++. The emulator is capable of running multiple NES games, provided as `.nes` ROM files. Users can switch between games by modifying the `.nes` file name in `main.cpp` before compiling.

## How to Run the Project

### Prerequisites

Ensure that you have the following installed on your system:
- g++ (MinGW or any compatible C++ compiler)
- Windows development libraries (OpenGL, GDI, WinMM, etc.)

### Steps to Compile and Run

1. **Place all source files in a single folder**

   `main.cpp`, `Bus.cpp`, `Bus.h`, `olc2C02.cpp`, `olc2C02.h`, `olc2A03.cpp`, `olc2A03.h`, `Mapper.cpp`, `Mapper.h`,  `Mapper_000.cpp`, `Mapper_000.h`, `Cartridge.cpp`, `Cartridge.h`, `olc6502.cpp`, `olc6502.h`, `olcPixelGameEngine.h`, `olcPGEX_Sound.h`

3. **Modify the `.nes` file being loaded**

   Open `main.cpp` and update the filename of the `.nes` ROM being loaded in the `OnUserCreate()` function:
     ```cpp
     bool OnUserCreate() override
     {
         // Load the cartridge
         cart = std::make_shared<Cartridge>("./ROMs/Super_mario_brothers.nes");
         ...
     }
     ```
   Replace `"Super_mario_brothers.nes"` with the desired `.nes` file.

4. **Open a terminal and navigate to the project folder.**

5. **Compile and link the project:**
   ```sh
   g++ main.cpp Bus.cpp olc2C02.cpp olc2A03.cpp Mapper.cpp Mapper_000.cpp Cartridge.cpp olc6502.cpp -o test -lopengl32 -lglu32 -lgdi32 -luser32 -lgdiplus -lshlwapi -ldwmapi -lwinmm
6. **Once the compilation is successful, run the program using:**
   ```sh
   .\test

### Changing the game

- The emulator runs NES ROM files found in the repository.
- To change the game, open main.cpp and modify the .nes filename being loaded in the OnUserCreate() function.
- Recompile and run the emulator to load the new game.

### Troubleshooting

- If compilation fails, ensure g++ is installed and added to your system's PATH.
- If missing libraries are reported, install the required Windows dependencies.
