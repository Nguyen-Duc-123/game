#ifndef BIRD_H
#define BIRD_H

#include <SDL2/SDL.h>

struct Bird {
    float x, y;
    float velocityY;

    Bird() {
        x = 800 / 4;
        y = 600 / 2;
        velocityY = 0;
    }

    void Jump() {
        velocityY = -9;
    }

    void Update() {
        velocityY += 0.55;
        y += velocityY;
        if (y < 0) y = 0;
        if (y > 600 - 50) y = 600 - 50;
    }
};

#endif // BIRD_H
