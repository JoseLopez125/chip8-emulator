#include <SDL2/SDL.h>
#include <iostream>
#include <chrono>
#include <cmath>

#include "Chip8.h"

// Audio settings
const int AMPLITUDE = 28000;
const int SAMPLE_RATE = 44100;

bool init();
void cleanup();
void updateDisplay();
bool handleInput();
void audioCallback(void* userdata, Uint8* stream, int len);

// Pointers to our window, renderer, and texture
SDL_Window* window{};
SDL_Renderer* renderer{};
SDL_Texture* texture{};

Chip8 chip8;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <ROM_FILE>" << std::endl;
        return 1;
    }

    if (!init()) return 1;

    if (!chip8.loadROM(argv[1])) {
        return 1; // Exit if ROM loading fails
    }

    auto lastFrameTime = std::chrono::high_resolution_clock::now();

    bool running = true;
    while (running) {
        running = handleInput();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastFrameTime).count();

        // 16.67 milliseconds ~60 Frames Per Second (60Hz)
        if (dt >= 16.67f) {
            lastFrameTime = currentTime;

            // Run the CPU 11 times per frame to achieve ~660Hz CPU speed (11 * 60 frames)
            for (int i = 0; i < 11; i++) {
                chip8.cycle();
            }

            // Update Chip8 timers and handle sound
            if (chip8.delayTimer > 0) chip8.delayTimer--;

            if (chip8.soundTimer > 0) {
                SDL_PauseAudio(0); // Play sound
                chip8.soundTimer--;
            } else {
                SDL_PauseAudio(1); // Pause sound
            }

            updateDisplay();
        }
    }

    cleanup();
    return 0;
}

void audioCallback(void* userdata, Uint8* stream, int len) {
    static double phase = 0.0;
    auto* buffer = (Sint16*)stream;
    int length = len / sizeof(Sint16);

    for (int i = 0; i < length; i++) {
        buffer[i] = (Sint16)(AMPLITUDE * sin(phase));
        phase += 2.0 * M_PI * 440.0 / SAMPLE_RATE; // 440 Hz tone
        if (phase >= 2.0 * M_PI) {
            phase -= 2.0 * M_PI;
        }
    }
}

bool handleInput() {
    SDL_Event e{};
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            return false;
        } else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
            // True if a key is pressed down, False if it was released
            bool isDown = (e.type == SDL_KEYDOWN);

            // The standard mapping from the original 16-key hex pad
            switch (e.key.keysym.sym) {
                case SDLK_1: chip8.keypad.at(0x1) = isDown; break;
                case SDLK_2: chip8.keypad.at(0x2) = isDown; break;
                case SDLK_3: chip8.keypad.at(0x3) = isDown; break;
                case SDLK_4: chip8.keypad.at(0xC) = isDown; break;
                case SDLK_q: chip8.keypad.at(0x4) = isDown; break;
                case SDLK_w: chip8.keypad.at(0x5) = isDown; break;
                case SDLK_e: chip8.keypad.at(0x6) = isDown; break;
                case SDLK_r: chip8.keypad.at(0xD) = isDown; break;
                case SDLK_a: chip8.keypad.at(0x7) = isDown; break;
                case SDLK_s: chip8.keypad.at(0x8) = isDown; break;
                case SDLK_d: chip8.keypad.at(0x9) = isDown; break;
                case SDLK_f: chip8.keypad.at(0xE) = isDown; break;
                case SDLK_z: chip8.keypad.at(0xA) = isDown; break;
                case SDLK_x: chip8.keypad.at(0x0) = isDown; break;
                case SDLK_c: chip8.keypad.at(0xB) = isDown; break;
                case SDLK_v: chip8.keypad.at(0xF) = isDown; break;
            }
        }
    }
    return true;
}

void updateDisplay() {
    // Update the texture
    void* pixels;
    int pitch;
    SDL_LockTexture(texture, NULL, &pixels, &pitch);

    // We take the void* from pixels and convert it to a uint32_t*
    // This allows us to update pixel data much easier
    auto* pixel_data = static_cast<uint32_t*>(pixels);
    for (int y = 0; y < chip8.SCREEN_HEIGHT; ++y) {
        for (int x = 0; x < chip8.SCREEN_WIDTH; ++x) {
            // Sets pixel data to match chip8 display pixels.
            // Pixels for chip 8 are on or off so we multiply by 255.
            uint8_t gray = chip8.display[y][x] * 255;
            pixel_data[y * (pitch / sizeof(uint32_t)) + x] = (0xFF << 24) | (gray << 16) | (gray << 8) | gray;
        }
    }
    SDL_UnlockTexture(texture);

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

bool init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Monochrome Display", 100, 100, 640, 320, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cout << "Error creating window: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        std::cout << "Error creating renderer: " << SDL_GetError() << std::endl;
        return false;
    }

    // Use nearest-neighbor scaling
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, chip8.SCREEN_WIDTH, chip8.SCREEN_HEIGHT);
    if (!texture) {
        std::cout << "Error creating texture: " << SDL_GetError() << std::endl;
        return false;
    }

    // Audio setup
    SDL_AudioSpec want, have;
    SDL_zero(want);
    want.freq = SAMPLE_RATE;
    want.format = AUDIO_S16;
    want.channels = 1;
    want.samples = 2048;
    want.callback = audioCallback;
    if (SDL_OpenAudio(&want, &have) < 0) {
        std::cout << "Error opening audio: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

void cleanup() {
    SDL_CloseAudio();
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
