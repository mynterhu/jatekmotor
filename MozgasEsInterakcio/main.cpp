#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>
#include <iostream>
#include <string>

struct Key {
    bool pickedUp = false;
    SDL_FRect rect {300, 350, 32, 32};
};

struct Door {
    bool isOpen = false;
    SDL_FRect rect {600, 250, 64, 128};

    void tryOpen(const Key& key, const SDL_FRect& playerRect) {
        if (!isOpen && key.pickedUp) {
            // Egyszerű "ütközés": ha a játékos közel van az ajtóhoz
            if (SDL_HasRectIntersectionFloat(&rect, &playerRect)) {
                isOpen = true;
                std::cout << "Az ajtó kinyílt!" << std::endl;
            }
        }
    }
};

struct Player {
    SDL_FRect rect {100, 300, 32, 32};
    float speed = 200.0f; // pixel/másodperc
};

// Textúra betöltése
SDL_Texture* LoadTexture(SDL_Renderer* renderer, const std::string& path) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "Nem sikerült betölteni a képet: " << path
                  << " - " << SDL_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    return texture;
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Edge Reversal - Player & Logic", 800, 600, SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

    if (!renderer) {
        std::cerr << "Renderer hiba: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Textúrák
    SDL_Texture* playerTex = LoadTexture(renderer, "assets/player.png");
    SDL_Texture* keyTex = LoadTexture(renderer, "assets/key.png");
    SDL_Texture* doorClosedTex = LoadTexture(renderer, "assets/door_closed.png");
    SDL_Texture* doorOpenTex = LoadTexture(renderer, "assets/door_open.png");

    if (!playerTex || !keyTex || !doorClosedTex || !doorOpenTex) {
        std::cerr << "Nem sikerült betölteni az egyik textúrát." << std::endl;
        return 1;
    }

    Player player;
    Key key;
    Door door;

    bool running = true;
    SDL_Event e;
    Uint64 lastTime = SDL_GetTicks();
    const float deltaTimeBase = 1.0f / 60.0f;

    while (running) {
        Uint64 currentTime = SDL_GetTicks();
        float delta = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        // --- Események ---
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT)
                running = false;
            if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE)
                running = false;
            if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_F)
                door.tryOpen(key, player.rect);
        }

        // --- Mozgás ---
        const bool* state = SDL_GetKeyboardState(nullptr);
        float dx = 0, dy = 0;

        if (state[SDL_SCANCODE_W]) dy -= 1;
        if (state[SDL_SCANCODE_S]) dy += 1;
        if (state[SDL_SCANCODE_A]) dx -= 1;
        if (state[SDL_SCANCODE_D]) dx += 1;

        player.rect.x += dx * player.speed * delta;
        player.rect.y += dy * player.speed * delta;

        // Kulcs felvétele
        if (!key.pickedUp && SDL_HasRectIntersectionFloat(&player.rect, &key.rect)) {
            key.pickedUp = true;
            std::cout << "Felvetted a kulcsot!" << std::endl;
        }

        // --- Render ---
        SDL_SetRenderDrawColor(renderer, 20, 25, 30, 255);
        SDL_RenderClear(renderer);

        SDL_Texture* doorTex = door.isOpen ? doorOpenTex : doorClosedTex;
        SDL_RenderTexture(renderer, doorTex, nullptr, &door.rect);

        if (!key.pickedUp)
            SDL_RenderTexture(renderer, keyTex, nullptr, &key.rect);

        SDL_RenderTexture(renderer, playerTex, nullptr, &player.rect);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyTexture(playerTex);
    SDL_DestroyTexture(keyTex);
    SDL_DestroyTexture(doorClosedTex);
    SDL_DestroyTexture(doorOpenTex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
