
#ifndef PLATFORM_LAYER_H
#define PLATFORM_LAYER_H

#include <SDL.h>
using cc = const char*;
using vc = void const*;


class Platform {
  public:
    Platform(cc title, int windowWidth, int windowHeight, int textureWidth, int textureHeight);
    ~Platform();

    private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;

    public:
    void Update(vc, int);
    bool ProcessInput(uint8_t*);
};



#endif //PLATFORM_LAYER_H
