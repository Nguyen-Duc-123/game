#include <load.h>
#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <data.h>
using namespace std;
SDL_Texture* LoadTexture(SDL_Renderer* renderer, const string& path) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, path.c_str());
    if (!texture) cout << "Khởi tạo IMG_LoadTexture thất bại: " << IMG_GetError() << endl;
    return texture;
}
void LoadMedia() {
    backgroundTexture = LoadTexture(renderer, "C:\\game\\assets\\images\\background1.png");
    birdTexture = LoadTexture(renderer, "C:\\game\\assets\\images\\bird.png");
    pipeTexture = LoadTexture(renderer, "C:\\game\\assets\\images\\pipe1.png");
    startTexture = LoadTexture(renderer, "C:\\game\\assets\\images\\start4.png");
    backgroundTexture2 = LoadTexture(renderer, "C:\\game\\assets\\images\\background10.png");
    backgroundTexture3 = LoadTexture(renderer, "C:\\game\\assets\\images\\background3.jpg");
    birdTextureNormal = IMG_LoadTexture(renderer, "C:\\game\\assets\\images\\bird_normal.png");
    birdTextureFly = IMG_LoadTexture(renderer, "C:\\game\\assets\\images\\bird_fly.png");
    birdTextureFall = IMG_LoadTexture(renderer, "C:\\game\\assets\\images\\bird_fall.png");


    pointSound = Mix_LoadWAV("C:\\game\\assets\\audio\\assets_audio_point.wav");
    jumpSound = Mix_LoadWAV("C:\\game\\assets\\audio\\assets_audio_fly.wav");
    if (!jumpSound) {
        cout << "Lỗi tải âm thanh nhảy: " << Mix_GetError() << endl;
    }
    Collision = Mix_LoadWAV("C:\\game\\assets\\audio\\assets_audio_collision.wav");
    if(!Collision){
        cout << "Lỗi tải âm thanh khi chạm cột " << Mix_GetError() << endl;    
    }
    font = TTF_OpenFont("C:\\game\\assets\\font\\font1.ttf", 24); 
    if (!font) {
    cout << "Load font không thành công " << TTF_GetError() << endl;
    }
}