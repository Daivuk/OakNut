#include "Sample.h"

#include <SceneManager.h>

onut::Game* onut::Game::CreateGame()
{
    return new Sample();
}

Sample::Sample()
{
}
