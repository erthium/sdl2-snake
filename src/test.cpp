// compiling with
// g++ -I /usr/include/SDL2 -lSDL2 test.cpp -o test

#include <iostream>
#include <chrono>
#include <thread>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>

#define TICK_INTERVAL 100

static Uint32 next_time;

Uint32 time_left(void)
{
    Uint32 now;

    now = SDL_GetTicks();
    if(next_time <= now)
        return 0;
    else
        return next_time - now;
}

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000

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

        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
    public:

    public:
        Game(){
            if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
                std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
            }
            window = SDL_CreateWindow("Snake",
                                                SDL_WINDOWPOS_CENTERED,
                                                SDL_WINDOWPOS_CENTERED,
                                                WINDOW_WIDTH, WINDOW_HEIGHT,
                                                0);
            if (window == NULL) { std::cout << "Window creation error: " << SDL_GetError() << std::endl;}
            renderer = SDL_CreateRenderer(window, -1, 0);
            if (renderer == NULL) { std::cout << "Renderer creation error: " << SDL_GetError() << std::endl;}
            std::cout << "Game initialized." << std::endl;
        }

        ~Game(){
            SDL_DestroyWindow(window);
            SDL_DestroyRenderer(renderer);
            SDL_Quit();
            std::cout << "Game deconstructed." << std::endl;
        }

        void createGame(int _width = 20, int _height = 20){
            width = _width;
            height = _height;
            gameOver = false;
            restart = false;
            // create the snake with the head and 2 bodu parts in a random position
            snakeSize = 1;
            snake[0][0] = rand() % width;
            snake[0][1] = rand() % height;
            snakeDirection = rand() % 4;
            createBerry();
        }

        void createBerry(){
            while(true){
                berryX = rand() % width;
                berryY = rand() % height;
                bool isBerryOnSnake = false;
                for(int i = 0; i < snakeSize; i++){
                    if(berryX == snake[i][0] && berryY == snake[i][1]){
                        isBerryOnSnake = true;
                        break;
                    }
                }
                if(!isBerryOnSnake){
                    break;
                }
            }
        }

        bool moveSneak(){
            // every loop, move the snake according to the current direction
            int* directionVec = getDirectionVec();

            // if the snake head is in the same position as the snake body, then game over
            for (int i = 1; i < snakeSize; i++){
                if (snake[0][0] + directionVec[0] == snake[i][0] && snake[0][1] + directionVec[1] == snake[i][1]){
                    delete[] directionVec;
                    std::cout << "Game Over!" << std::endl; 
                    return false;
                }
            }

            // if the snake head is in the same position as the berry, then eat the berry
            if (snake[0][0] + directionVec[0] == berryX && snake[0][1] + directionVec[1] == berryY){
                snakeSize++;
                for (int i = snakeSize; i > 0; i--){
                    snake[i+1][0] = snake[i][0];
                    snake[i+1][1] = snake[i][1];
                }
                snake[0][0] = berryX;
                snake[0][1] = berryY;
                createBerry();
                delete[] directionVec;
                return true;
            }

            // or else, just move the snake body
            for (int i = snakeSize - 1; i > 0; i--){
                snake[i][0] = snake[i-1][0];
                snake[i][1] = snake[i-1][1];
            }
            snake[0][0] += directionVec[0];
            snake[0][1] += directionVec[1];
            delete[] directionVec;
            return true;
        }

        int* getDirectionVec(){
            int* vec  = new int[2];
            switch (snakeDirection) {
                case 0:  // Left
                    vec[0] = -1;
                    vec[1] = 0;
                    break;
                case 1:  // Up
                    vec[0] = 0;
                    vec[1] = -1;
                    break;
                case 2:  // Right
                    vec[0] = 1;
                    vec[1] = 0;
                    break;
                case 3:  // Down
                    vec[0] = 0;
                    vec[1] = 1;
                    break;
                default:
                    // Handle invalid direction
                    std::cerr << "Invalid direction: " << snakeDirection << std::endl;
                    delete[] vec; // Clean up the allocated memory
                    return nullptr; // Return nullptr in case of an error
            }
            return vec;
        }

        void handleEvents(){
            SDL_Event event;
            while(SDL_PollEvent(&event)){
                switch (event.type)
                {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_LEFT:
                        snakeDirection = 0;
                        break;
                    case SDLK_a:
                        snakeDirection = 0;
                        break;
                    case SDLK_UP:
                        snakeDirection = 1;
                        break;
                    case SDLK_w:
                        snakeDirection = 1;
                        break;
                    case SDLK_RIGHT:
                        snakeDirection = 2;
                        break;
                    case SDLK_d:
                        snakeDirection = 2;
                        break;
                    case SDLK_DOWN:
                        snakeDirection = 3;
                        break;
                    case SDLK_s:
                        snakeDirection = 3;
                        break;
                    case SDLK_r:
                        restart = true;
                        break;
                    case SDLK_ESCAPE:
                        pause = !pause;
                        break;
                    default:
                        break;
                    }
                    break;
                
                default:
                    break;
                }
            }

        }

        void renderGame(){
            
            if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255) != 0)
            {
                std::cout << "Set renderer color error: " << SDL_GetError() << std::endl;
            }
            SDL_RenderClear(renderer);
            int x_unit = WINDOW_WIDTH / width;
            int y_unit = WINDOW_HEIGHT / height;

            SDL_Rect rect;
            rect.x = 0;
            rect.y = 0;
            rect.w = WINDOW_WIDTH;
            rect.h = WINDOW_HEIGHT;
            SDL_RenderFillRect(renderer, &rect);
            // render the snake
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            for (int i = 0; i < snakeSize; i++){
                rect.x = snake[i][0] * x_unit;
                rect.y = snake[i][1] * y_unit;
                rect.w = x_unit;
                rect.h = y_unit;
                SDL_RenderFillRect(renderer, &rect);
            }
            // render the berry
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            rect.x = berryX * x_unit;
            rect.y = berryY * y_unit;
            rect.w = x_unit;
            rect.h = y_unit;
            SDL_RenderFillRect(renderer, &rect);
            // render the grid
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            for (int i = 0; i < width; i++)
            {
                SDL_RenderDrawLine(renderer, x_unit * i, 0, x_unit * i, WINDOW_HEIGHT);  
            }
            for (int i = 0; i < height; i++)
            {
                SDL_RenderDrawLine(renderer, 0, y_unit * i, WINDOW_WIDTH, y_unit * i);
            }
            SDL_RenderPresent(renderer);
        }

        void loop(){
            next_time = SDL_GetTicks() + TICK_INTERVAL;
            while(!quit){
                if(!pause && !gameOver){ gameOver = !moveSneak(); }
                if (restart){ createGame(width, height); }
                
                handleEvents();
                renderGame();
                SDL_Delay(time_left());
                next_time += TICK_INTERVAL;
            }
            std::cout << "Closing the Game." << std::endl;
        }
};

Game *game = nullptr;

int main()
{    
    game = new Game();
    game->createGame();
    game->loop();

    delete game;
    return 0;
}
