#include <SDL2/SDL.h>
#include <iostream>

#include "Chip8.h"

bool init();

void kill();

bool loop();

// Pointers to our window, renderer, and texture
SDL_Window* window{};
SDL_Renderer* renderer{};
SDL_Texture* texture{};

Chip8 chip8;

// State for key press
bool g_keyPressed = false;

int main(int argc, char *argv[]) {
    if (!init()) return 1;

    chip8.loadROM("2-ibm-logo.ch8");

    for (int i = 0; i < 10; ++i) {
        chip8.cycle();
    }

    // main loop
    while (loop()) {
        SDL_Delay(100);
    }

    kill();
    return 0;
}

bool loop() {
    SDL_Event e{};
    while (SDL_PollEvent(&e) != 0) {
        switch (e.type) {
            case SDL_QUIT: // SDL_QUIT allows the app to quit
                return false;
            default:
                break;
        }
    }

    chip8.cycle();

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

    return true;
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

    return true;
}

void kill() {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
