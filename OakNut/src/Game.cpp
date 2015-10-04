#include "Game.h"

onut::Game* onut::Game::s_pGame = nullptr;

onut::Game::Game()
{
    s_pGame = this;
}

onut::Game* onut::Game::getGame()
{
    return s_pGame;
}

onut::Game::~Game()
{
}
