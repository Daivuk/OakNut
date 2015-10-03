#include "Dispatcher.h"
#include "Game.h"
#include "IRenderer.h"
#include "SceneManager.h"
#include "Timing.h"
#include "IWindow.h"

onut::Game* onut::Game::s_pGame = nullptr;
onut::Dispatcher* onut::Game::s_pDispatcher = nullptr;
onut::IRenderer* onut::Game::s_pRenderer = nullptr;
onut::SceneManager* onut::Game::s_pSceneManager = nullptr;
onut::Timing* onut::Game::s_pTiming = nullptr;
onut::IWindow* onut::Game::s_pWindow = nullptr;

onut::Game* onut::Game::getGame()
{
    return s_pGame;
}

onut::Dispatcher* onut::Game::getMainDispatcher()
{
    return s_pDispatcher;
}

onut::IRenderer* onut::Game::getRenderer()
{
    return s_pRenderer;
}

onut::SceneManager* onut::Game::getSceneManager()
{
    return s_pSceneManager;
}

onut::Timing* onut::Game::getTiming()
{
    return s_pTiming;
}

onut::IWindow* onut::Game::getWindow()
{
    return s_pWindow;
}

onut::Game::~Game()
{
}
