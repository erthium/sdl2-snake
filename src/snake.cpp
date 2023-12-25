#include "game.hpp"


Game *game = nullptr;

int main()
{    
    game = new Game();
    game->createGame();
    game->loop();

    delete game;
    return 0;
}
