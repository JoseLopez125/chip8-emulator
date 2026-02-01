#pragma once

#include <array>
#include <cstdint>
#include <random>
#include <string>

class Chip8 {
public:
    static constexpr unsigned int SCREEN_WIDTH = 64;
    static constexpr unsigned int SCREEN_HEIGHT = 32;

    // Display (64x32 monochrome pixels)
    std::array<std::array<uint8_t, SCREEN_WIDTH>, SCREEN_HEIGHT> display{};

    Chip8();

    bool loadROM(const std::string &filename);
    void cycle();

private:
    static constexpr unsigned int MEMORY_SIZE = 4096;
    static constexpr unsigned int REGISTER_COUNT = 16;
    static constexpr unsigned int ROM_OFFSET = 512; // ROM starts at 0x200 (512) in memory
    static constexpr unsigned int STACK_SIZE = 16;

    // Hardware
    std::array<uint8_t, REGISTER_COUNT> registers{}; // 16 Registers called V0...VF
    std::array<uint8_t, MEMORY_SIZE> memory{};       // RAM
    uint16_t index{};                                // Index Register (I)
    uint16_t pc{ROM_OFFSET};                         // Program Counter (points at current instruction)
    std::array<uint16_t, STACK_SIZE> stack{};        // Stack for subroutine calls
    uint8_t sp{0};                                   // Stack pointer
    uint8_t delayTimer{};                            // Timer to achieve 60Hz
    uint8_t soundTimer{};                            // Gives off beeping sound when != 0
    uint16_t opcode{};                               // Current instruction to be decoded from chip8

    // Helpers to generate random numbers for opcode 0xCXNN
    std::default_random_engine randGen;
    std::uniform_int_distribution<uint8_t> randByte;
};
