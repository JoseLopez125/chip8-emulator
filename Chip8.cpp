#include "Chip8.h"

#include <fstream>
#include <iostream>

Chip8::Chip8() = default;

void Chip8::cycle() {
    // Fetch instruction from memory at the current PC (program counter)
    // opcodes are 16-bit so we use bitwise operators
    opcode = (static_cast<uint16_t>(memory[pc]) << 8) | memory[pc + 1];

    // TEMPORARY DEBUG: Print what the CPU sees
    // printf("PC: 0x%04X | Opcode: 0x%04X\n", pc, opcode);

    pc += 2; // move pc to next instruction

    // Decode and execute opcode
    switch (opcode & 0xF000) {
        case 0x0000: // opcode: 0x0___
            switch (opcode) {
            case 0x00e0: // opcode: 0x00e0
                    // Clear the display
                    for (auto& row : display)
                        std::fill(row.begin(), row.end() , 0);
                    break;
            }
            break;
        case 0x1000: // opcode: 0x1NNN
            // Jump to address NNN
            pc = opcode & 0x0FFF;
            break;
        case 0x6000: // opcode: 0x6XNN
            // Set register VX to NN
            registers.at((opcode & 0x0F00) >> 8) = opcode & 0x00FF;
            break;
        case 0x7000: // opcode: 0x7XNN
            // Add NN to register VX
            registers.at((opcode & 0x0F00) >> 8) += opcode & 0x00FF;
            break;
        case 0xA000: // opcode: 0xANNN
            // Sets index register I to address NNN
            index = opcode & 0x0FFF;
            break;
        case 0xD000: // opcode: 0xDXYN
            // Draw sprite to coordinate (VX, VY) with a width of 8 pixels and a
            // height of N pixels. Sprite starts at memory location I. X and y
            // are the top left corner of the sprite.
            uint8_t x = registers.at((opcode & 0x0F00) >> 8) % SCREEN_WIDTH;
            uint8_t y = registers.at((opcode & 0x00F0) >> 4) % SCREEN_HEIGHT;
            uint8_t height = opcode & 0x000F;

            registers.at(15) = 0; // VF set to 0
            // Iterate through each row of the sprite
            for (uint8_t row = 0; row < height && y + row < SCREEN_HEIGHT; row++) {
                // Iterate through each pixel in the row
                uint8_t spriteByte = memory.at(index + row);
                for (uint8_t col = 0; col < 8 && x + col < SCREEN_WIDTH; col++) {
                    // Check if the pixel in the sprite is set to 1
                    if ((spriteByte & (0x80 >> col)) != 0) {
                        // Check if the pixel in the display is also set to 1
                        if (display.at(y + row).at(x + col) == 1) {
                            registers.at(16) = 1; // VF set to 1
                        }
                        // XOR the pixel in the display with the sprite
                        display.at(y + row).at(x + col) ^= 1;
                    }
                }
            }
            break;
    }
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

    file.close();

    return true;
}
