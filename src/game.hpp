#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>


class Game{
    private:
        int width;
        int height;
        int berryX, berryY;
        int snake[50][2];
        int snakeSize;
        int snakeDirection;
        bool gameOver;
        bool restart;
        bool quit;
        bool pause;
        bool directionChanged;

        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
    public:

    public:
        Game();
        ~Game();
        void createGame(int _width = 15, int _height = 15);
        void createBerry();
        bool moveSneak();
        int* getDirectionVec();
        bool changeDirectionTo(int dir);
        void handleEvents();
        void renderGame();
        void loop();
};
