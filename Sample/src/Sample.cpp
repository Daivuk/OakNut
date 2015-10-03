#include "Sample.h"

onut::Game* onut::Game::CreateGame()
{
    return new Sample();
}

Sample::Sample()
{
    setName("Sample Game");
}
