#include "Chip8.h"

#include <fstream>
#include <iostream>

Chip8::Chip8() : randGen(std::random_device{}()), randByte(0, 255) {}

void Chip8::cycle() {
    // Fetch instruction from memory at the current PC (program counter)
    // opcodes are 16-bit so we use bitwise operators
    opcode = (static_cast<uint16_t>(memory[pc]) << 8) | memory[pc + 1];

    // TEMPORARY DEBUG: Print what the CPU sees
    // printf("PC: 0x%04X | Opcode: 0x%04X\n", pc, opcode);

    pc += 2; // move pc to next instruction

    // Decode and execute opcode
    switch (opcode & 0xF000) {
        case 0x0000:
            // opcode: 0x0___
            switch (opcode) {
                case 0x00e0:
                    // opcode: 0x00e0
                    // Clear the display
                    for (auto &row: display)
                        std::fill(row.begin(), row.end(), 0);
                    break;
                case 0x00ee:
                    // opcode: 0x00EE
                    // Return from subroutine
                    pc = stack.at(--sp);
                    break;
            }
            break;
        case 0x1000: // opcode: 0x1NNN
            // Jump to address NNN
            pc = opcode & 0x0FFF;
            break;
        case 0x2000: // opcode: 0x2NNN
            // Call subroutine at NNN
            stack.at(sp++) = pc;
            pc = opcode & 0x0FFF;
            break;
        case 0x3000: // opcode: 0x3XNN
            // Skip next instruction if VX == NN
            if (registers.at((opcode & 0x0F00) >> 8) == (opcode & 0x00FF)) {
                pc += 2;
            }
            break;
        case 0x4000: // opcode: 0x4XNN
            // Skip next instruction if VX != NN
            if (registers.at((opcode & 0x0F00) >> 8) != (opcode & 0x00FF)) {
                pc += 2;
            }
            break;
        case 0x5000: // opcode: 0x5XY0
            // Skip next instruction if VX == VY
            if (registers.at((opcode & 0x0F00) >> 8) == registers.at((opcode & 0x00F0) >> 4)) {
                pc += 2;
            }
            break;
        case 0x6000: // opcode: 0x6XNN
            // Set register VX to NN
            registers.at((opcode & 0x0F00) >> 8) = opcode & 0x00FF;
            break;
        case 0x7000: // opcode: 0x7XNN
            // Add NN to register VX
            registers.at((opcode & 0x0F00) >> 8) += opcode & 0x00FF;
            break;
        case 0x8000: {
            // opcode: 0x8___
            switch (opcode & 0x000F) {
                case 0x0000: // opcode: 0x8XY0
                    // Set VX to VY
                    registers.at((opcode & 0x0F00) >> 8) = registers.at((opcode & 0x00F0) >> 4);
                    break;
                case 0x0001: // opcode: 0x8XY1
                    // Set VX to VX OR VY
                    registers.at((opcode & 0x0F00) >> 8) |= registers.at((opcode & 0x00F0) >> 4);
                    break;
                case 0x0002: // opcode: 0x8XY2
                    // Set VX to VX AND VY
                    registers.at((opcode & 0x0F00) >> 8) &= registers.at((opcode & 0x00F0) >> 4);
                    break;
                case 0x0003: // opcode: 0x8XY3
                    // Set VX to VX XOR VY
                    registers.at((opcode & 0x0F00) >> 8) ^= registers.at((opcode & 0x00F0) >> 4);
                    break;
                case 0x0004: {
                    // opcode: 0x8XY4
                    // Adds VY to VX. If there is an overflow set VF 1 and 0 if not
                    uint8_t vx = registers.at((opcode & 0x0F00) >> 8);
                    uint8_t vy = registers.at((opcode & 0x00F0) >> 4);
                    int tempResult = vx + vy;
                    registers.at((opcode & 0x0f00) >> 8) += vy;
                    registers.at(15) = 0; // Set VF to 0
                    if (tempResult > UINT8_MAX) {
                        registers.at(15) = 1;
                    }
                    break;
                }
                case 0x0005: {
                    // opcode: 0x8XY5
                    // Subtracts VY from VX. If there is an underflow set VF
                    // to 0 and 1 if not
                    uint8_t vx = registers.at((opcode & 0x0F00) >> 8);
                    uint8_t vy = registers.at((opcode & 0x00F0) >> 4);
                    registers.at((opcode & 0x0F00) >> 8) -= vy;
                    registers.at(15) = 0;
                    if (vx >= vy) {
                        registers.at(15) = 1;
                    }
                    break;
                }
                case 0x0006: {
                    // opcode: 0x8XY6
                    // Shift VX to the right by 1, and store the shifted bit
                    // into VF
                    uint8_t vx = registers.at((opcode & 0x0F00) >> 8);
                    registers.at((opcode & 0x0F00) >> 8) >>= 1;
                    registers.at(15) = vx & 0x1;
                    break;
                }
                case 0x0007: {
                    // opcode: 0x8XY7
                    // Set VX to VY - VX. If there is an underflow set VF to
                    // 0 and 1 if not
                    uint8_t vx = registers.at((opcode & 0x0F00) >> 8);
                    uint8_t vy = registers.at((opcode & 0x00F0) >> 4);
                    registers.at((opcode & 0x0F00) >> 8) = vy - vx;
                    registers.at(15) = 0;
                    if (vy >= vx) {
                        registers.at(15) = 1;
                    }
                    break;
                }
                case 0x000E: {
                    // opcode: 0x8XYE
                    // Shift VX to the left by 1, and store the shifted bit
                    // into VF
                    uint8_t vx = registers.at((opcode & 0x0F00) >> 8);
                    registers.at((opcode & 0x0F00) >> 8) <<= 1;
                    registers.at(15) = (vx & 0x80) >> 7;
                    break;
                }
            }
            break;
        }
        case 0x9000: // opcode: 0x9XY0
            // Skip next instruction if VX != VY
            if (registers.at((opcode & 0x0F00) >> 8) != registers.at((opcode & 0x00F0) >> 4)) {
                pc += 2;
            }
            break;
        case 0xA000: // opcode: 0xANNN
            // Sets index register I to address NNN
            index = opcode & 0x0FFF;
            break;
        case 0xB000: // opcode: 0xBNNN
            // Jumps to the address NNN
            pc = (opcode & 0x0FFF) + registers.at(0);
            break;
        case 0xC000: {
            // opcode: 0xCXNN
            // Sets VX to a random number between 0 and 255 with a mask NN
            registers.at((opcode & 0x0F00) >> 8) = randByte(randGen) & (opcode & 0x00FF);
            break;
        }
        case 0xD000: {
            // opcode: 0xDXYN
            // Draw sprite to coordinate (VX, VY) with a width of 8 pixels and a
            // height of N pixels. Sprite starts at memory location I. X and y
            // are the top left corner of the sprite.
            uint8_t x = registers.at((opcode & 0x0F00) >> 8) % SCREEN_WIDTH;
            uint8_t y = registers.at((opcode & 0x00F0) >> 4) % SCREEN_HEIGHT;
            uint8_t height = opcode & 0x000F;

            // Iterate through each row of the sprite
            for (uint8_t row = 0; row < height && y + row < SCREEN_HEIGHT; row++) {
                // Iterate through each pixel in the row
                uint8_t spriteByte = memory.at(index + row);
                for (uint8_t col = 0; col < 8 && x + col < SCREEN_WIDTH; col++) {
                    // Check if the pixel in the sprite is set to 1
                    if ((spriteByte & (0x80 >> col)) != 0) {
                        // XOR the pixel in the display with the sprite
                        display.at(y + row).at(x + col) ^= 1;

                        // Set flag VF
                        if (display.at(y + row).at(x + col) == 1) {
                            registers.at(15) = 1; // VF set to 1
                        } else {
                            registers.at(15) = 0; // VF set to 0
                        }
                    }
                }
            }
            break;
        }
        case 0xF000: {
            // opcode: 0xF___
            switch (opcode & 0x00FF) {
                case 0x001E: // opcode: 0xFX1E
                    // Add VX to index register I
                    index += registers.at((opcode & 0x0F00) >> 8);
                    break;
                case 0x0033: {
                    // opcode: 0xFX33
                    // Store VX as a three decimal digit with each digit in a
                    // separate memory location at I
                    // ex. if vx contains 156 (0x86), it would put the number 1,
                    // at address in I, 5 in address I+1, and 6 in address I+2
                    int vx = registers.at((opcode & 0x0F00) >> 8);
                    memory.at(index) = vx / 100;
                    memory.at(index + 1) = (vx / 10) % 10;
                    memory.at(index + 2) = vx % 10;
                    break;
                }
                case 0x0055: {
                    // opcode: 0xFX55
                    // Stores from V0 to VX (inclusive) in memory starting at
                    // address I. Each register gets its own memory address
                    uint8_t x = (opcode & 0x0F00) >> 8;
                    for (uint8_t i = 0; i <= x; i++) {
                        memory.at(index + i) = registers.at(i);
                    }
                    break;
                }
                case 0x0065: {
                    // opcode: 0xFX65
                    // Fills from V0 to VX (inclusive) with values from memory,
                    // starting at address I.
                    uint8_t x = (opcode & 0x0F00) >> 8;
                    for (uint8_t i = 0; i <= x; i++) {
                        registers.at(i) = memory.at(index + i);
                    }
                    break;
                }
            }
            break;
        }
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
