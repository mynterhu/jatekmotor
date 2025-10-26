#include <SDL3/SDL.h>
#include <iostream>
#include <algorithm>

int main(int argc, char* argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL init hiba: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Edge Reversal - Grafika",
                                          1280, 720,
                                          SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, "direct3d12");

    if (!renderer) {
        std::cerr << "Renderer hiba: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    bool running = true;
    SDL_Event event;

    // Játékos négyzet pozíciója
    float playerX = 640.0f;
    float playerY = 360.0f;
    float speed = 4.0f; // mozgási sebesség

    while (running) {
        // 1️⃣ Események kezelése
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
                running = false;
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDL_SCANCODE_ESCAPE)
                running = false;
        }

        // 2️⃣ Billentyűzet állapot lekérdezése
        const bool* state = SDL_GetKeyboardState(nullptr);
        if (state[SDL_SCANCODE_W]) playerY -= speed;
        if (state[SDL_SCANCODE_S]) playerY += speed;
        if (state[SDL_SCANCODE_A]) playerX -= speed;
        if (state[SDL_SCANCODE_D]) playerX += speed;

        
        // 3️⃣ Rajzolás
        SDL_SetRenderDrawColor(renderer, 15, 15, 25, 255); // háttér
        SDL_RenderClear(renderer);

        SDL_FRect playerRect = { playerX - 25, playerY - 25, 50, 50 };
        SDL_SetRenderDrawColor(renderer, 50, 200, 255, 255); // világoskék négyzet
        SDL_RenderFillRect(renderer, &playerRect);

        SDL_RenderPresent(renderer);

        SDL_Delay(16); // kb. 60 FPS
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
