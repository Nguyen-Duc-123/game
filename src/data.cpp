#include <data.h>
#include <iostream>
#include <SDL2/SDL.h>
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const float GRAVITY = 0.55;
const int JUMP_STRENGTH = 9;
const int PIPE_SPEED = 3;
const int PIPE_GAP = 160;
const int PIPE_WIDTH = 80;
int highScore = 0;
int score = 0;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* backgroundTexture = nullptr;
SDL_Texture* backgroundTexture2 = nullptr;
SDL_Texture* backgroundTexture3 = nullptr;
SDL_Texture* birdTexture = nullptr;
SDL_Texture* pipeTexture = nullptr;
SDL_Texture* startTexture = nullptr;
SDL_Texture* birdTextureNormal = nullptr;
SDL_Texture* birdTextureFly = nullptr;
SDL_Texture* birdTextureFall = nullptr;
SDL_Texture* gameOverBackground = nullptr;


Mix_Chunk* Collision = nullptr;
Mix_Chunk* jumpSound = nullptr;
Mix_Chunk* pointSound = nullptr;
TTF_Font *font = nullptr;
