#include "Sample.h"

#include <SceneManager.h>

onut::Game* onut::Game::CreateGame()
{
    return new Sample();
}

Sample::Sample()
{
    setName("Sample Game");
}

void Sample::onLoaded()
{
    onut::Game::getSceneManager()->loadScene("assets/scenes/sample.json");
}
