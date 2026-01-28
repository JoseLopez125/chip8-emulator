#include <SDL2/SDL.h>
#include <iostream>

bool init();

void kill();

bool loop();

// Pointers to our window and surface
SDL_Window *window{};
SDL_Surface *winSurface{};

int main(int argc, char *argv[]) {
    if (!init()) return 1;

    // main loop
    while (loop()) {
        SDL_Delay(10);
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
            case SDL_KEYDOWN:
                // Color surface red
                SDL_FillRect(winSurface, NULL, SDL_MapRGB(winSurface->format, 255, 90, 120));
                break;
            case SDL_KEYUP:
                // Color surface black
                SDL_FillRect(winSurface, NULL, SDL_MapRGB(winSurface->format, 0, 0, 0));
                break;
            default:
                break;
        }
    }

    SDL_UpdateWindowSurface(window);
    return true;
}

bool init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Display", 99, 100, 640, 480, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cout << "Error creating window: " << SDL_GetError() << std::endl;
        return false;
    }

    winSurface = SDL_GetWindowSurface(window);
    if (!winSurface) {
        std::cout << "Error getting surface: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

void kill() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}
