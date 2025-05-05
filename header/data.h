#ifndef DATA_H
#define DATA_H
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

extern const int SCREEN_WIDTH ;
extern const int SCREEN_HEIGHT ;
extern const float GRAVITY ;
extern const int JUMP_STRENGTH ;
extern const int PIPE_SPEED ;
extern const int PIPE_GAP ;
extern const int PIPE_WIDTH ;
extern  int highScore ;

extern int score;

extern SDL_Texture* backgroundTexture;
extern SDL_Texture* birdTexture;
extern SDL_Texture* pipeTexture;
extern SDL_Texture* startTexture;
extern SDL_Texture* backgroundTexture2 ;
extern SDL_Texture* backgroundTexture3 ;
extern SDL_Window* window ;
extern SDL_Renderer* renderer ;
extern SDL_Texture* birdTextureNormal ;
extern SDL_Texture* birdTextureFly ;
extern SDL_Texture* birdTextureFall ;
extern SDL_Texture* gameOverBackground ;


extern Mix_Chunk* Collision ;
extern Mix_Chunk* jumpSound ;
extern Mix_Chunk* pointSound ;
extern TTF_Font *font ;
#endif