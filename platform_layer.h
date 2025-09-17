
#ifndef PLATFORM_LAYER_H
#define PLATFORM_LAYER_H

#include <SDL.h>
#include <SDL_audio.h>
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
    SDL_AudioSpec* FallenDown; 
    SDL_AudioStream* Track; 

    public:
    bool bFallenDown; 
    void Update(vc, int);
    bool ProcessInput(uint8_t*);
};



#endif //PLATFORM_LAYER_H
