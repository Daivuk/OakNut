#include "Camera.h"
#include "ContentManager.h"
#include "DirectionalLight.h"
#include "AmbientLight.h"
#include "Dispatcher.h"
#include "EntityFactory.h"
#include "Game.h"
#include "Main.h"
#include "MeshRenderer.h"
#include "ObjectLibrary.h"
#include "PointLight.h"
#include "Random.h"
#include "Renderer.h"
#include "Renderer_d3d11.h"
#include "SceneManager.h"
#include "Timing.h"
#include "Window.h"
#include "Window_win.h"

void onut::Main::main()
{
    // Register classes to be loaded dynamically
    REG_COMPONENT(AmbientLight);
    REG_COMPONENT(Camera);
    REG_COMPONENT(ContentManager);
    REG_COMPONENT(DirectionalLight);
    REG_COMPONENT(Dispatcher);
    REG_COMPONENT(MeshRenderer);
    REG_COMPONENT(PointLight);
    REG_COMPONENT(SceneManager);
    REG_COMPONENT(Timing);
#if defined(ONUT_RENDERER_D3D11)
    REG_COMPONENT2(Renderer_d3d11, Renderer);
#endif
#if defined(WIN32)
    REG_COMPONENT2(Window_win, Window);
#endif

    randomizeSeed();

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
