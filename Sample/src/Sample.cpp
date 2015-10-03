#include "Sample.h"

onut::IGame* onut::IGame::CreateGame()
{
    return new Sample();
}
