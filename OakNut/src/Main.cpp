#include "Dispatcher.h"
#include "Game.h"
#include "IRenderer.h"
#include "IWindow.h"
#include "Main.h"
#include "SceneManager.h"
#include "Timing.h"

void onut::Main::main()
{
    onut::Game::s_pGame = Game::CreateGame();
    onut::Game::s_pWindow = IWindow::createWindow();
    onut::Game::s_pDispatcher = new onut::Dispatcher();
    onut::Game::s_pTiming = new onut::Timing();
    onut::Game::s_pRenderer = IRenderer::createRenderer(onut::Game::s_pWindow);
    onut::Game::s_pSceneManager = new onut::SceneManager();

    onut::Game::s_pGame->retain();

    // Load game properties from game.json
    onut::Game::s_pGame->loadPropertiesFromFile("assets/game.json");

    // Add base components to it
    onut::Game::s_pGame->addComponent(onut::Game::s_pWindow);
    onut::Game::s_pGame->addComponent(onut::Game::s_pTiming);
    onut::Game::s_pGame->addComponent(onut::Game::s_pDispatcher);
    onut::Game::s_pGame->addComponent(onut::Game::s_pSceneManager);
    onut::Game::s_pGame->addComponent(onut::Game::s_pRenderer);

    onut::Game::s_pGame->onLoaded();

    while (onut::Game::s_pWindow->getEnabled())
    {
        onut::Game::s_pGame->onUpdate();
        onut::Game::s_pGame->onCreate(); // This will call newly created components if not already initialized
        onut::Game::s_pGame->onDraw();
    }

    onut::Game::s_pGame->release();
}
