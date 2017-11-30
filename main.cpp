#include "Game.hpp"
#include "GameStateStart.hpp"

int main()
{
    Game game;

    game.pushState(new GameStateStart(&game));
    game.run();

    return 0;
}
