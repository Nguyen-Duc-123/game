// #include <bits/stdc++.h>
// #include <SDL2/SDL.h>
// #include <SDL2/SDL_image.h>
// #include <SDL2/SDL_mixer.h>
// #include <SDL2/SDL_ttf.h>
// #include <load.h>
// #include <data.h>
// #include <render.h>
// #include <pipes.h>
// #include <bird.h
// using namespace std;
// void Render() {
//     SDL_RenderClear(renderer);
//     if (score >= 10 && score < 30 && backgroundTexture2) {
//         SDL_RenderCopy(renderer, backgroundTexture2, nullptr, nullptr);
//     } else if(score >= 30 && backgroundTexture3) {
//         SDL_RenderCopy(renderer, backgroundTexture3, nullptr, nullptr);
//     }
//     else {
//         SDL_RenderCopy(renderer , backgroundTexture , nullptr, nullptr );
//     }
//     for (auto& pipe : pipes) {
//         SDL_Rect topPipe = {pipe.x, 0, PIPE_WIDTH, pipe.height};
//         SDL_RenderCopyEx(renderer, pipeTexture, nullptr, &topPipe, 0, nullptr, SDL_FLIP_VERTICAL);
//         SDL_Rect bottomPipe = {pipe.x, pipe.height + PIPE_GAP, PIPE_WIDTH, SCREEN_HEIGHT - pipe.height - PIPE_GAP};
//         SDL_RenderCopy(renderer, pipeTexture, nullptr, &bottomPipe);
//     }

//     SDL_Rect birdRect = {(int)bird.x, (int)bird.y, 45, 45}; // Vẽ con chim
//     SDL_RenderCopy(renderer, birdTexture, nullptr, &birdRect);
//     RenderScore(); 
//     SDL_RenderPresent(renderer); 
// }