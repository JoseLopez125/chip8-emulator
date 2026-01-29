#include "Chip8.h"

#include <fstream>
#include <iostream>

Chip8::Chip8() = default;

void Chip8::cycle() {
    // Fetch instruction from memory at the current PC (program counter)
    // opcodes are 16-bit so we use bitwise operators
    opcode = (static_cast<uint16_t>(memory[pc]) << 8) | memory[pc + 1];

    // TEMPORARY DEBUG: Print what the CPU sees
    printf("PC: 0x%04X | Opcode: 0x%04X\n", pc, opcode);

    pc += 2; // move pc to next instruction

    // TODO: Decode and execute opcode
}

bool Chip8::loadROM(const std::string &filename) {
    // Open the file as a stream of binary data, and move the file pointer to the
    // end.
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open ROM file " << filename << std::endl;
        return false;
    }

    // Get the size of the file and reposition the file pointer to the beginning.
    std::streampos size = file.tellg();
    file.seekg(0, std::ios::beg);

    if (size > MEMORY_SIZE - ROM_OFFSET) {
        std::cerr << "Error: ROM file is too large." << std::endl;
        return false;
    }

    // Read the ROM data into the Chip-8's memory.
    // reinterpret_cast<char*> takes forces the datatype to be "relabeled as char*"
    // without changing the data.
    // .data() gives us a raw pointer to the underlying array.
    file.read(reinterpret_cast<char *>(memory.data() + ROM_OFFSET), size);

    // TEMPORARY DEBUG: Print first 10 bytes of code
    std::cout << "ROM in Memory:" << std::endl;
    for (int i = 0; i < 10; ++i) {
        // 0x200 is 512 (Start of Chip-8 programs)
        printf("0x%02X ", memory[512 + i]);
    }
    std::cout << std::endl;

    file.close();

    return true;
}
