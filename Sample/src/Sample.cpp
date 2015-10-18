#include "LevelGenerator.h"
#include "Rotate.h"
#include "Sample.h"
#include "Torch.h"

#include <ObjectLibrary.h>
#include <SceneManager.h>

onut::Game* onut::Game::CreateGame()
{
    return new Sample();
}

Sample::Sample()
{
    REG_COMPONENT(LevelGenerator);
    REG_COMPONENT(Rotate);
    REG_COMPONENT(Torch);
}
