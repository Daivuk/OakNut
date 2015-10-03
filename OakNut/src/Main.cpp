#include "IGame.h"
#include "IRenderer.h"
#include "IWindow.h"
#include "Main.h"
#include "Timing.h"

void onut::Main::main()
{
    // Create the game using the factory method that the user has to define
    auto pGame = IGame::CreateGame();
    pGame->retain();

    // Add base components to it
    auto pWindow = IWindow::createWindow();
    pGame->addComponent(pWindow);
    pGame->addComponent(new onut::Timing());
    pGame->addComponent(IRenderer::createRenderer(pWindow));

    while (pWindow->isEnabled())
    {
        pGame->onCreate(); // This will call newly created components if not already initialized
        pGame->onUpdate();
        pGame->onDraw();
    }

    pGame->release();
}
