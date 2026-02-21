# Chip-8 Emulator (C++)

A complete, cycle-accurate Chip-8 interpreter written in modern C++20 using SDL2 for graphics, input, and audio. This project successfully emulates the Chip-8 virtual machine, capable of running original Chip-8 programs with sound.

<!-- TODO: Add a GIF of the emulator running a ROM -->

## Table of Contents
- [Project Goal](#project-goal)
- [Features](#features)
- [Building and Running](#building-and-running)
- [Controls](#controls)
- [Technical Roadmap](#technical-roadmap)
- [Future Improvements](#future-improvements)
- [A Note on ROMs](#a-note-on-roms)

## Project Goal
This project was a deep dive into systems programming and computer architecture. The goal was to build a fully functional emulator from scratch to master the "Fetch-Decode-Execute" cycle, memory management, and the low-level bitwise operations required for emulation.

## Features
*   **Complete Instruction Set:** All 35 original Chip-8 opcodes are fully implemented.
*   **SDL2 Rendering & Input:** Utilizes SDL2 for a scalable, crisp monochrome display and responsive keyboard input.
*   **Sound Support:** Emulates the Chip-8's single-tone buzzer using SDL2's audio API.
*   **Command-Line Interface:** ROMs are loaded via a simple command-line argument.
*   **Modern C++:** Written in C++20, using modern features for clean, robust, and maintainable code.

## Building and Running

### 1. Install Dependencies
*   **C++ Compiler:** A modern compiler supporting C++20 (GCC, Clang, MSVC).
*   **Build System:** CMake (version 3.15 or newer).
*   **Library:** SDL2 (Simple DirectMedia Layer).

**Linux**
*   **Fedora / RHEL:** `sudo dnf install SDL2-devel cmake`
*   **Ubuntu / Debian:** `sudo apt-get install libsdl2-dev cmake`

**macOS**
*   Using [Homebrew](https://brew.sh/): `brew install sdl2 cmake`

**Windows**
*   Using [vcpkg](https://vcpkg.io/): `vcpkg install sdl2:x64-windows`

### 2. Compile and Run

Clone the repository and create a build directory.
```bash
git clone https://github.com/JoseLopez125/chip8-emulator.git
cd chip8-emulator
mkdir build && cd build
```

Run CMake to configure the project. **On Windows, you must add the vcpkg toolchain file argument.**

**Linux / macOS:**
```bash
cmake ..
```

**Windows:**
```bash
cmake .. -DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake
```

To build the project and run it with a ROM:

```bash
cmake --build .

# Example of how to run a test ROM
./chip8_emulator ../roms/test-roms/1-chip8-logo.ch8

# Example of how to run a game
./chip8_emulator ../roms/Brix/Brix.ch8
```

## Controls
The original Chip-8 used a 16-key hexadecimal keypad. This emulator maps those keys to a modern keyboard in a 4x4 grid layout:

| Chip-8 Key | Keyboard |
| :---: | :---: |
| `1` `2` `3` `C` | `1` `2` `3` `4` |
| `4` `5` `6` `D` | `Q` `W` `E` `R` |
| `7` `8` `9` `E` | `A` `S` `D` `F` |
| `A` `0` `B` `F` | `Z` `X` `C` `V` |

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

## Future Improvements
*   **Implement a GUI:** Use a library like Dear ImGui to add controls for pausing, resetting, and loading ROMs without restarting.
*   **Configurable Speeds:** Allow the CPU and timer frequencies to be adjusted.
*   **Save/Load State:** Implement functionality to save and restore the emulator's state at any point.
*   **Super Chip-8 Support:** Extend the emulator to support the additional instructions and higher resolution of the Super Chip-8.

## A Note on ROMs
This project includes a selection of public domain test ROMs for demonstration and testing purposes. The original software for commercial Chip-8 systems is subject to copyright law and is not included here.
