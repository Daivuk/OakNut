#include "Camera.h"
#include "ContentManager.h"
#include "Dispatcher.h"
#include "EntityFactory.h"
#include "Game.h"
#include "Renderer.h"
#include "Window.h"
#include "Main.h"
#include "MeshRenderer.h"
#include "ObjectLibrary.h"
#include "Renderer_d3d11.h"
#include "SceneManager.h"
#include "Timing.h"
#include "Window_win.h"

void onut::Main::main()
{
    // Register classes to be loaded dynamically
    ObjectLibrary::registerObject<Camera>("Camera");
    ObjectLibrary::registerObject<ContentManager>("ContentManager");
    ObjectLibrary::registerObject<Dispatcher>("Dispatcher");
    ObjectLibrary::registerObject<MeshRenderer>("MeshRenderer");
    ObjectLibrary::registerObject<SceneManager>("SceneManager");
    ObjectLibrary::registerObject<Timing>("Timing");
#if defined(WIN32)
    ObjectLibrary::registerObject<Window_win>("Window");
#endif
#if defined(ONUT_RENDERER_D3D11)
    ObjectLibrary::registerObject<Renderer_d3d11>("Renderer");
#endif

    auto pGame = Game::CreateGame();
    pGame->retain();

    // Load game properties from game.json
    // This file is mendatory
    pGame->loadPropertiesFromFile(pGame->getFilename());
    pGame->onCreate(); // Force load base components

    auto pWindow = pGame->getComponent<Window>();
    auto pTiming = pGame->getComponent<Timing>();
    if (pWindow && pTiming)
    {
        // Main loop
        while (pWindow->getEnabled())
        {
            pGame->onUpdate(pTiming->getTimeInfo());
            pGame->onCreate(); // This will call newly created components if not already initialized
            pGame->onDraw();
        }
    }

    // Free up
    EntityFactory::clear();
    pGame->release();
    ObjectLibrary::clear();
}
