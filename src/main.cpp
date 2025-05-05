#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <bird.h>
#include <data.h>
#include <load.h>
#include <Close.h>
#include <init.h>
using namespace std;

enum GameState { MENU, PLAY, HIGHSCORE };
GameState gameState = MENU;

struct Pipe {
    int x, height;
    bool passed = false;
};


vector<Pipe> pipes;
Bird bird;
SDL_Rect startButton = { 320, 100, 200, 60 };
SDL_Rect highScoreButton = { 320, 250, 200, 60 };
SDL_Rect quitButton = { 320, 500, 200, 60 };


int pipeSpeed = PIPE_SPEED;


void GeneratePipe() {   // Thêm các cột vào vector
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


void LoadHighScore() {
    ifstream file("highscore.txt");
    if (file.is_open()) {
        file >> highScore;
        file.close();
    } 
}
void SaveHighScore() {
    ofstream file("highscore.txt");
    if (file.is_open()) {
        file << highScore << endl;
        file.close();
    } 
}

void RenderHighScore() {
    SDL_RenderClear(renderer); // Xóa màn hình trước khi vẽ high score
    SDL_Color textColor = {255, 255, 255};
    string hsText = "High Score: " + to_string(highScore);
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, hsText.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    int textWidth = textSurface->w;
    int textHeight = textSurface->h;
    SDL_Rect textRect = { 400 - textWidth / 2, 300 - textHeight / 2, textWidth, textHeight };

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    // Thêm một dòng hướng dẫn để quay lại MENU (tùy chọn)
    string returnText = "Press any key to return";
    SDL_Surface* returnSurface = TTF_RenderText_Solid(font, returnText.c_str(), textColor);
    SDL_Texture* returnTexture = SDL_CreateTextureFromSurface(renderer, returnSurface);
    SDL_Rect returnRect = { 400 - (int)returnSurface->w / 2, 400, returnSurface->w, returnSurface->h };
    SDL_RenderCopy(renderer, returnTexture, NULL, &returnRect);
    SDL_FreeSurface(returnSurface);
    SDL_DestroyTexture(returnTexture);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    SDL_RenderPresent(renderer);
}


void Render() {
    SDL_RenderClear(renderer);
    if (score >= 10 && score < 30 && backgroundTexture2) {
        SDL_RenderCopy(renderer, backgroundTexture2, nullptr, nullptr);
    } else if(score >= 30 && backgroundTexture3) {
        SDL_RenderCopy(renderer, backgroundTexture3, nullptr, nullptr);
    }
    else {
        SDL_RenderCopy(renderer , backgroundTexture , nullptr, nullptr );
    }
    for (auto& pipe : pipes) {
        SDL_Rect topPipe = {pipe.x, 0, PIPE_WIDTH, pipe.height};
        SDL_RenderCopyEx(renderer, pipeTexture, nullptr, &topPipe, 0, nullptr, SDL_FLIP_VERTICAL);
        SDL_Rect bottomPipe = {pipe.x, pipe.height + PIPE_GAP, PIPE_WIDTH, SCREEN_HEIGHT - pipe.height - PIPE_GAP};
        SDL_RenderCopy(renderer, pipeTexture, nullptr, &bottomPipe);
    }

    SDL_Texture* currentBirdTexture = birdTextureNormal;

    if (bird.velocityY < -2) {
        currentBirdTexture = birdTextureFly;     // Bay lên
    } else if (bird.velocityY > 2) {
        currentBirdTexture = birdTextureFall;     // Rơi xuống
    }

    SDL_Rect birdRect = {(int)bird.x, (int)bird.y, 50, 50};
    SDL_RenderCopy(renderer, currentBirdTexture, nullptr, &birdRect);

    RenderScore();
    SDL_RenderPresent(renderer);
}


void HandleEvents(SDL_Event& e) {
    if (e.type == SDL_QUIT) {
        exit(0);
    }
    else if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_SPACE) {
            if (gameState == MENU) {
                gameState = PLAY;
            } else if (gameState == PLAY) {
                bird.Jump();
                if (jumpSound) {
                    Mix_PlayChannel(-1, jumpSound, 0);
                }
            } else if (gameState == HIGHSCORE) {
                gameState = MENU; // Quay lại menu khi nhấn phím bất kỳ ở màn hình Highscore
            }
        }
    }
    else if (e.type == SDL_MOUSEBUTTONDOWN) {
        int x = e.button.x;
        int y = e.button.y;

        if (gameState == MENU) {
            if (x >= startButton.x && x <= startButton.x + startButton.w && y >= startButton.y && y <= startButton.y + startButton.h) {
                gameState = PLAY;
            }
            // Xử lý nút Highscore
            else if (x >= highScoreButton.x && x <= highScoreButton.x + highScoreButton.w && y >= highScoreButton.y && y <= highScoreButton.y + highScoreButton.h) {
                gameState = HIGHSCORE;
            }
            else if (x >= quitButton.x && x <= quitButton.x + quitButton.w && y >= quitButton.y && y <= quitButton.y + quitButton.h) {
                exit(0);
            }
        }
        else if (gameState == HIGHSCORE) {
            gameState = MENU; // Quay lại menu khi nhấn chuột ở màn hình Highscore
        }
    }
}


bool CheckCollision() {         // Xử lí va chạm
    for (auto& pipe : pipes) {
        if (bird.x + 45 > pipe.x && bird.x < pipe.x + PIPE_WIDTH) {
            if (bird.y < pipe.height || bird.y + 45 > pipe.height + PIPE_GAP) {
                return true;
            }
        }
    }
    return bird.y >= SCREEN_HEIGHT - 50;
}

void Update() {
    bird.Update();
    if (score > 5) pipeSpeed = PIPE_SPEED + 2;
    else if(score > 10) pipeSpeed = PIPE_SPEED + 3;
    else pipeSpeed = PIPE_SPEED;
    for (auto& pipe : pipes) {
        pipe.x -= PIPE_SPEED;
        if (!pipe.passed && bird.x > pipe.x + PIPE_WIDTH) {
            pipe.passed = true;
            score++;
            Mix_PlayChannel(-1 , pointSound , 0);
        }
    }
    if (!pipes.empty() && pipes[0].x + PIPE_WIDTH < 0) {
        pipes.erase(pipes.begin());
    }
    if (pipes.empty() || pipes.back().x < SCREEN_WIDTH - 250) {
        GeneratePipe();
    }
    if (CheckCollision()) {
        cout << "Game Over! Score: " << score << ", High Score hiện tại: " << highScore << endl;
        if (score > highScore) {
            highScore = score;
            SaveHighScore();
        }
        if(Collision){
            Mix_PlayChannel(-1 , Collision , 0);
        }
        SDL_Delay(2000);
        gameState = MENU;
        bird = Bird();
        pipes.clear();
        score = 0 ;
        LoadHighScore();
    }
}

int main(int argc, char* argv[]) {
    if (!Init()) return -1;
    LoadMedia();
    LoadHighScore(); // Tải điểm cao nhất khi khởi động game

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
        } else if (gameState == HIGHSCORE) {
            RenderHighScore();
        }

        SDL_Delay(16);
    }

    Close();
    return 0;
}