# Chip-8 Emulator (C++)

A complete, cycle-accurate Chip-8 interpreter written in modern C++20 using SDL2 for graphics, input, and audio. This project successfully emulates the Chip-8 virtual machine, capable of running original Chip-8 programs with sound.

<!-- TODO: Add a GIF of the emulator running a ROM -->

## Project Goal
This project was a deep dive into systems programming and computer architecture. The goal was to build a fully functional emulator from scratch to master the "Fetch-Decode-Execute" cycle, memory management, and the low-level bitwise operations required for emulation.

## Features
*   **Complete Instruction Set:** All 35 original Chip-8 opcodes are fully implemented.
*   **SDL2 Rendering & Input:** Utilizes SDL2 for a scalable, crisp monochrome display and responsive keyboard input.
*   **Sound Support:** Emulates the Chip-8's single-tone buzzer using SDL2's audio API.
*   **Command-Line Interface:** ROMs are loaded via a simple command-line argument.
*   **Modern C++:** Written in C++20, using modern features for clean, robust, and maintainable code.

## Controls
The original Chip-8 used a 16-key hexadecimal keypad. This emulator maps those keys to a modern keyboard in a 4x4 grid layout:

| Chip-8 Key | Keyboard |
| :---: | :---: |
| `1` `2` `3` `C` | `1` `2` `3` `4` |
| `4` `5` `6` `D` | `Q` `W` `E` `R` |
| `7` `8` `9` `E` | `A` `S` `D` `F` |
| `A` `0` `B` `F` | `Z` `X` `C` `V` |

## A Note on ROMs
This emulator does not include any ROM files. The original software for the Chip-8 platform is subject to copyright law. To use this emulator, you must provide your own ROM files, which should only be from sources you are legally entitled to use (e.g., public domain or homebrew games).

## Technical Roadmap
*   **Phase 1: Core Architecture** (Complete)
    *   Setup SDL2 rendering loop.
    *   Implement memory mapping (4KB RAM).
*   **Phase 2: The CPU** (Complete)
    *   Implement the 16 8-bit registers (V0-VF) and 16-bit Index register.
    *   Build the Fetch-Decode-Execute cycle.
    *   Implement all 35 Chip-8 opcodes.
*   **Phase 3: Graphics & Peripherals** (Complete)
    *   Implement 64x32 monochrome display logic (XOR sprite drawing).
    *   Map hex keypad input to modern keyboard.
    *   Implement delay and sound timers (60Hz timing).
    *   Implement buzzer sound via SDL2 audio.

## Dependencies
*   **C++ Compiler:** GCC or Clang (supporting C++20)
*   **Build System:** CMake
*   **Library:** SDL2 (Simple DirectMedia Layer)

## Building the Project

### 1. Install Dependencies
**Fedora / RHEL:**
```bash
sudo dnf install SDL2-devel cmake
```
**Ubuntu / Debian:**
```bash
sudo apt-get install libsdl2-dev cmake
```

### 2. Compile and Run
Clone the repository, create a build directory, and then run `cmake`:
```bash
mkdir build && cd build
cmake ..
cmake --build .
./chip8_emulator path/to/your/rom.ch8
```

## Future Improvements
*   **Implement a GUI:** Use a library like Dear ImGui to add controls for pausing, resetting, and loading ROMs without restarting.
*   **Configurable Speeds:** Allow the CPU and timer frequencies to be adjusted.
*   **Save/Load State:** Implement functionality to save and restore the emulator's state at any point.
*   **Super Chip-8 Support:** Extend the emulator to support the additional instructions and higher resolution of the Super Chip-8.
