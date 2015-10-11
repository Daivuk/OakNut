#include "Rotate.h"
#include "Sample.h"

#include <ObjectLibrary.h>
#include <SceneManager.h>

onut::Game* onut::Game::CreateGame()
{
    return new Sample();
}

Sample::Sample()
{
    onut::ObjectLibrary::registerObject<Rotate>("Rotate");
}
