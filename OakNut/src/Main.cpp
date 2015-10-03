#include "Game.h"
#include "IRenderer.h"
#include "IWindow.h"
#include "Main.h"
#include "Timing.h"

void onut::Main::main()
{
    // Create the game using the factory method that the user has to define
    auto pGame = Game::CreateGame();
    pGame->retain();

    // Our window component
    auto pWindow = IWindow::createWindow();
    pWindow->setWindowName(pGame->getName());

    // Add base components to it
    pGame->addComponent(pWindow);
    pGame->addComponent(new onut::Timing());
    pGame->addComponent(IRenderer::createRenderer(pWindow));

    while (pWindow->getEnabled())
    {
        pGame->onUpdate();
        pGame->onCreate(); // This will call newly created components if not already initialized
        pGame->onDraw();
    }

    pGame->release();
}
