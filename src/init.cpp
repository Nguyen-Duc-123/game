#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <load.h>
#include <data.h>
using namespace std;

bool Init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) return false;

    if (!IMG_Init(IMG_INIT_PNG)) return false;

    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == 0) {
        cout << "Khởi tạo SDL_Mixer thất bại " << Mix_GetError() << endl;
        return false;
    }    
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cout << "Khởi tạo SDL_Mixer thất bại: " << Mix_GetError() << endl;
        return false;
    }
    if(TTF_Init()==-1){
        cout << "Khởi tạo SDL_ttf thất bại"<<endl;
        return false;
    }
    window = SDL_CreateWindow("Flappy Bird", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) return false;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) return false;

    srand(time(0));
    return true;
}