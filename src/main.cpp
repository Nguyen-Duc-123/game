#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <bird.h>
#include <data.h>
using namespace std;

enum GameState { MENU, PLAY };
GameState gameState = MENU;

struct Pipe {
    int x, height;
    bool passed = false;
};

vector<Pipe> pipes;
Bird bird;

Mix_Chunk* Collision = nullptr;
Mix_Chunk* jumpSound = nullptr;
Mix_Chunk* pointSound = nullptr;
int score = 0;
int highscore = 0;
TTF_Font *font = nullptr;

SDL_Texture* LoadTexture(SDL_Renderer* renderer, const string& path) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, path.c_str());
    if (!texture) cout << "IMG_LoadTexture Error: " << IMG_GetError() << endl;
    return texture;
}
bool Init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) return false;

    if (!IMG_Init(IMG_INIT_PNG)) return false;

    //  Khởi tạo SDL_mixer
    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == 0) {
        cout << "Khởi tạo SDL_Mixer thất bại " << Mix_GetError() << endl;
        return false;
    }    
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cout << "Khởi tạo SDL_Mixer thất bại: " << Mix_GetError() << endl;
        return false;
    }
    // Khởi tạo SDL_ttf
    if(TTF_Init()==-1){
        cout << "Khởi tạo SDL_ttf thất bại"<<endl;
        return false;
    }
    // Khởi tạo cửa sổ
    window = SDL_CreateWindow("Flappy Bird", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) return false;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) return false;

    srand(time(0));
    return true;
}


void LoadMedia() {
    backgroundTexture = LoadTexture(renderer, "C:\\game\\assets\\images\\background10.png");
    birdTexture = LoadTexture(renderer, "C:\\game\\assets\\images\\bird.png");
    pipeTexture = LoadTexture(renderer, "C:\\game\\assets\\images\\pipe1.png");
    startTexture = LoadTexture(renderer, "C:\\game\\assets\\images\\start2.png");

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

void Close() {
    Mix_FreeChunk(jumpSound);
    Mix_CloseAudio();
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(birdTexture);
    SDL_DestroyTexture(pipeTexture);
    SDL_DestroyTexture(startTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void GeneratePipe() {
    int minHeight = 50;
    int maxHeight = SCREEN_HEIGHT - PIPE_GAP - minHeight;
    int height = minHeight + rand() % (maxHeight - minHeight);
    pipes.push_back({SCREEN_WIDTH, height});
}

void RenderStartScreen() {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, startTexture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

void RenderScore(){
    SDL_Color color = {255 , 255 , 255};
    string scoreText = "Score: " + to_string(score);
    SDL_Surface* textSurface = TTF_RenderText_Solid(font,scoreText.c_str(), color);
    if(!textSurface){
        cout << "Không thể tải được textSurface";
        return;
    } 
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    if (!textTexture) {
        cout << "Lỗi tạo texture từ surface: " << SDL_GetError() << endl;
        return;
    }
    SDL_Rect dstRect = { 20, 20, 0, 0 };
    SDL_QueryTexture(textTexture, NULL, NULL, &dstRect.w, &dstRect.h);

    // Vẽ điểm số lên màn hình
    SDL_RenderCopy(renderer, textTexture, NULL, &dstRect);

    SDL_DestroyTexture(textTexture);

}

void Render() {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);

    for (auto& pipe : pipes) {
        SDL_Rect topPipe = {pipe.x, 0, PIPE_WIDTH, pipe.height};
        SDL_RenderCopyEx(renderer, pipeTexture, nullptr, &topPipe, 0, nullptr, SDL_FLIP_VERTICAL);
        SDL_Rect bottomPipe = {pipe.x, pipe.height + PIPE_GAP, PIPE_WIDTH, SCREEN_HEIGHT - pipe.height - PIPE_GAP};
        SDL_RenderCopy(renderer, pipeTexture, nullptr, &bottomPipe);
    }

    SDL_Rect birdRect = {(int)bird.x, (int)bird.y, 50, 50};
    SDL_RenderCopy(renderer, birdTexture, nullptr, &birdRect);
    RenderScore(); 
    SDL_RenderPresent(renderer); 
}


void HandleEvents(SDL_Event& e) {
    if (e.type == SDL_QUIT) {
        exit(0);
    } else if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_SPACE) {
            if (gameState == MENU) {
                gameState = PLAY;
            } else if (gameState == PLAY) {
                bird.Jump();
                if (jumpSound) {
                    Mix_PlayChannel(-1, jumpSound, 0);
                }
            }
        }
    }
}

bool CheckCollision() {
    for (auto& pipe : pipes) {
        if (bird.x + 50 > pipe.x && bird.x < pipe.x + PIPE_WIDTH) {
            if (bird.y < pipe.height || bird.y + 50 > pipe.height + PIPE_GAP) {
                return true;
            }
        }
    }
    return bird.y >= SCREEN_HEIGHT - 50;
}

void Update() {
    bird.Update();

    for (auto& pipe : pipes) {
        pipe.x -= PIPE_SPEED;

        if (!pipe.passed && bird.x > pipe.x + PIPE_WIDTH) {
            pipe.passed = true;
            score++;
        }
    }

    if (!pipes.empty() && pipes[0].x + PIPE_WIDTH < 0) {
        pipes.erase(pipes.begin());
    }

    if (pipes.empty() || pipes.back().x < SCREEN_WIDTH - 250) {
        GeneratePipe();
    }

    if (CheckCollision()) {
        if(Collision){
            Mix_PlayChannel(-1 , Collision , 0);
        }
        SDL_Delay(2000);
        gameState = MENU;
        bird = Bird();
        pipes.clear();
        score = 0 ;
    }
}

int main(int argc, char* argv[]) {
    if (!Init()) return -1;
    LoadMedia();

    SDL_Event e;

    while (true) {
        while (SDL_PollEvent(&e)) {
            HandleEvents(e);
        }

        if (gameState == MENU) {
            RenderStartScreen();
        } else if (gameState == PLAY) {
            Update();
            Render();
        }

        SDL_Delay(16);
    }

    Close();
    return 0;
}