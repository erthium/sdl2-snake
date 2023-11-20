#include <iostream>
#include <chrono>
#include <thread>

using namespace std;

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
    public:

    public:
        Game(){

        }

        ~Game(){

        }

        void createGame(int _width = 10, int _height = 10){
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

        void showGame(){
            
        }

        void loop(){
            while(!quit){
                if (gameOver){ cout << "Game Over!" << endl; }
                else { gameOver = !moveSneak(); }
                if (restart){ createGame(width, height); }
                
                this_thread::sleep_for(chrono::milliseconds(100));
            }
            cout << "Closing the Game." << endl;
        }
};


int main()
{
    Game game;
    game.loop();
    return 0;
}
