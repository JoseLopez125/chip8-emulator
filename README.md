# Chip-8 Emulator (C++)

A Cycle-Accurate Chip-8 interpreter written in C++20 using SDL2 for rendering.

## Project Goal
This project serves as a deep dive into systems programming and computer architecture. The goal is to build a functional emulator from scratch to understand the "Fetch-Decode-Execute" cycle, memory management, and bitwise operations required for low-level software development.

**Current Status:** Initialization & Rendering Engine (Active Development)

## Technical Roadmap
* **Phase 1: Core Architecture** (In Progress)
    * Setup SDL2 rendering loop and input handling.
    * Implement memory mapping (4KB RAM).
* **Phase 2: The CPU**
    * Implement the 16 8-bit registers (V0-VF) and 16-bit Index register.
    * Build the Fetch-Decode-Execute cycle.
    * Implement the 35 Chip-8 opcodes.
* **Phase 3: Graphics & Peripherals**
    * Implement 64x32 monochrome display logic (XOR sprite drawing).
    * Map hex keypad input to modern keyboard.
    * Implement delay and sound timers (60Hz timing).

## Dependencies
* **C++ Compiler:** GCC or Clang (supporting C++20)
* **Build System:** CMake
* **Library:** SDL2 (Simple DirectMedia Layer)

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
./chip8_emulator
```

