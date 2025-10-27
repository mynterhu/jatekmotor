#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>
#include <iostream>
#include <vector>

struct Wall {
    SDL_FRect rect;
};

struct Player {
    SDL_FRect rect {100, 300, 32, 32};
    float speed = 200.0f;

    void move(float dx, float dy, float delta, const std::vector<Wall>& walls) {
        SDL_FRect newPos = rect;
        newPos.x += dx * speed * delta;
        newPos.y += dy * speed * delta;

        // Ellenőrizzük, ütközne-e valamelyik fallal
        bool collision = false;
        for (const auto& wall : walls) {
            if (SDL_HasRectIntersectionFloat(&newPos, &wall.rect)) {
                collision = true;
                break;
            }
        }

        if (!collision)
            rect = newPos;
    }
};

SDL_Texture* LoadTexture(SDL_Renderer* renderer, const std::string& path) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "Nem sikerült betölteni: " << path << " (" << SDL_GetError() << ")\n";
        return nullptr;
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    return tex;
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Edge Reversal - Collision Demo", 800, 600, SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

    SDL_Texture* playerTex = LoadTexture(renderer, "assets/player.png");

    // Falak létrehozása
    std::vector<Wall> walls = {
        {{200, 200, 400, 32}},   // vízszintes fal
        {{200, 400, 32, 200}},   // függőleges fal
        {{600, 300, 64, 64}}     // blokk
    };

    Player player;
    bool running = true;
    SDL_Event e;
    Uint64 lastTime = SDL_GetTicks();

    while (running) {
        Uint64 currentTime = SDL_GetTicks();
        float delta = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT)
                running = false;
        }

        const bool* state = SDL_GetKeyboardState(nullptr);
        float dx = 0, dy = 0;
        if (state[SDL_SCANCODE_W]) dy -= 1;
        if (state[SDL_SCANCODE_S]) dy += 1;
        if (state[SDL_SCANCODE_A]) dx -= 1;
        if (state[SDL_SCANCODE_D]) dx += 1;

        player.move(dx, dy, delta, walls);

        // --- Rajzolás ---
        SDL_SetRenderDrawColor(renderer, 25, 25, 30, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 200, 50, 50, 255);
        for (auto& wall : walls)
            SDL_RenderFillRect(renderer, &wall.rect);

        SDL_RenderTexture(renderer, playerTex, nullptr, &player.rect);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyTexture(playerTex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
