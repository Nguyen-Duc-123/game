#ifndef DATA_H
#define DATA_H
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

extern const int SCREEN_WIDTH ;
extern const int SCREEN_HEIGHT ;
extern const float GRAVITY ;
extern const int JUMP_STRENGTH ;
extern const int PIPE_SPEED ;
extern const int PIPE_GAP ;
extern const int PIPE_WIDTH ;

extern SDL_Texture* backgroundTexture;
extern SDL_Texture* birdTexture;
extern SDL_Texture* pipeTexture;
extern SDL_Texture* startTexture;
extern SDL_Window* window ;
extern SDL_Renderer* renderer ;
extern Mix_Music* music;
extern Mix_Chunk* Sound;
#endif