#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>
#include <iostream>
#include <string>

// Logikai objektumok
struct Key {
    bool pickedUp = false;
    SDL_FRect rect{200, 300, 32, 32};
};

struct Door {
    bool isOpen = false;
    SDL_FRect rect{600, 250, 64, 128};

    void tryOpen(const Key& key) {
        if (key.pickedUp) isOpen = true;
    }
};

// Segédfüggvény a textúra betöltésére
SDL_Texture* LoadTexture(SDL_Renderer* renderer, const std::string& path) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "Nem sikerült betölteni a képet: " << path
                  << " - " << SDL_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);

    if (!texture) {
        std::cerr << "Nem sikerült textúrát készíteni: " << SDL_GetError() << std::endl;
    }
    
    return texture;
}

int main() {
    // SDL inicializálás hibakezelés nélkül
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Edge Reversal - Sprite Demo", 800, 600, SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "Ablak létrehozási hiba: " << SDL_GetError() << std::endl;
        return 1;
    }
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "Renderer hiba: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        return 1;
    }

    // Textúrák betöltése
    SDL_Texture* doorClosedTex = LoadTexture(renderer, "assets/door_closed.png");
    SDL_Texture* doorOpenTex = LoadTexture(renderer, "assets/door_open.png");
    SDL_Texture* keyTex = LoadTexture(renderer, "assets/key.png");

    if (!doorClosedTex || !doorOpenTex || !keyTex) {
        std::cerr << "Hiányzó textúra!" << std::endl;

        // Takarítás
        if (doorClosedTex) SDL_DestroyTexture(doorClosedTex);
        if (doorOpenTex) SDL_DestroyTexture(doorOpenTex);
        if (keyTex) SDL_DestroyTexture(keyTex);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        return 1;
    }

    Key key;
    Door door;
    bool running = true;
    SDL_Event e;

    while (running) {
        // --- Input ---
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT)
                running = false;

            if (e.type == SDL_EVENT_KEY_DOWN) {
                if (e.key.key == SDLK_ESCAPE)
                    running = false;

                if (e.key.key == SDLK_E)  // E = kulcs felvétel
                    key.pickedUp = true;

                if (e.key.key == SDLK_F)  // F = ajtó nyitás
                    door.tryOpen(key);
            }
        }

        // --- Render ---
        SDL_SetRenderDrawColor(renderer, 20, 20, 25, 255);
        SDL_RenderClear(renderer);

        // Ajtó kirajzolása
        SDL_Texture* currentDoor = door.isOpen ? doorOpenTex : doorClosedTex;
        SDL_RenderTexture(renderer, currentDoor, nullptr, &door.rect);

        // Kulcs csak akkor látszik, ha még nem vettük fel
        if (!key.pickedUp)
            SDL_RenderTexture(renderer, keyTex, nullptr, &key.rect);

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    // --- Takarítás ---
    SDL_DestroyTexture(doorClosedTex);
    SDL_DestroyTexture(doorOpenTex);
    SDL_DestroyTexture(keyTex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}