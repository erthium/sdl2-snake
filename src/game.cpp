#include "game.hpp"

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


Game::Game(){
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


Game::~Game(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game deconstructed." << std::endl;
}


void Game::createGame(int _width, int _height){
    width = _width;
    height = _height;
    gameOver = false;
    restart = false;
    // create the snake with the head and 2 bodu parts in a random position
    snakeSize = 3;
    snake[0][0] = rand() % width;
    snake[0][1] = rand() % height;
    snakeDirection = rand() % 4;
    int* dir = getDirectionVec();
    snake[1][0] = snake[0][0] - dir[0];
    snake[1][1] = snake[0][1] - dir[1];
    snake[2][0] = snake[1][0] - dir[0];
    snake[2][1] = snake[1][1] - dir[1];
    delete[] dir;
    createBerry();
}


void Game::createBerry(){
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


bool Game::moveSneak(){
    // every loop, move the snake according to the current direction
    int* directionVec = getDirectionVec();
    
    // move the head 
    snake[0][0] += directionVec[0];
    snake[0][1] += directionVec[1];

    // if the head is over the grid, teleport it to the other side
    if (snake[0][0] < -1) snake[0][0] = width - 1;
    else if (snake[0][0] >= width + 1) snake[0][0] = 0;
    else if (snake[0][1] < -1) snake[0][1] = height - 1;
    else if (snake[0][1] >= height + 1) snake[0][1] = 0;

    // if the snake head is in the same position as the berry, then eat the berry
    if (snake[0][0] == berryX && snake[0][1] == berryY){
        snake[snakeSize][0] = snake[snakeSize - 1][0] - directionVec[0];
        snake[snakeSize][1] = snake[snakeSize - 1][1] - directionVec[1];
        snakeSize++;
        createBerry();
    }

    // then just move the snake body
    for (int i = snakeSize - 1; i > 1; i--){
        snake[i][0] = snake[i-1][0];
        snake[i][1] = snake[i-1][1];
    }
    snake[1][0] = snake[0][0] - directionVec[0];
    snake[1][1] = snake[0][1] - directionVec[1];

    // if the snake head is in the same position as the snake body, then game over
    for (int i = 1; i < snakeSize; i++){
        if (snake[0][0] + directionVec[0] == snake[i][0] && snake[0][1] + directionVec[1] == snake[i][1]){
            delete[] directionVec;
            std::cout << "Game Over!" << std::endl; 
            return false;
        }
    }

    delete[] directionVec;
    return true;
}


int* Game::getDirectionVec(){
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


bool Game::changeDirectionTo(int dir){
    if (directionChanged) return false;
    if (dir == 0 && snakeDirection != 2){
        snakeDirection = 0;
        directionChanged = true;
        return true;
    }
    else if (dir == 1 && snakeDirection != 3){
        snakeDirection = 1;
        directionChanged = true;                
        return true;
    }
    else if (dir == 2 && snakeDirection != 0){
        snakeDirection = 2;
        directionChanged = true;
        return true;
    }
    else if (dir == 3 && snakeDirection != 1){
        snakeDirection = 3;
        directionChanged = true;
        return true;
    }
    return false;
}


void Game::handleEvents(){
    directionChanged = false;
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
                changeDirectionTo(0);
                break;
            case SDLK_a:
                changeDirectionTo(0);
                break;
            case SDLK_UP:
                changeDirectionTo(1);
                break;
            case SDLK_w:
                changeDirectionTo(1);
                break;
            case SDLK_RIGHT:
                changeDirectionTo(2);
                break;
            case SDLK_d:
                changeDirectionTo(2);
                break;
            case SDLK_DOWN:
                changeDirectionTo(3);
                break;
            case SDLK_s:
                changeDirectionTo(3);
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


void Game::renderGame(){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
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


void Game::loop(){
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
