#include "Entity.h"
#include "Game.h"
#include "IRenderer.h"
#include "MeshRenderer.h"

void onut::MeshRenderer::onDraw()
{
    if (!getMesh() || !getMaterial() || !getComponentManager()) return;
    auto pEntity = dynamic_cast<Entity*>(getComponentManager());
    if (!pEntity) return;

    Game::getGame()->getComponent<IRenderer>()->draw(getMesh(), getMaterial(), pEntity->getWorldMatrix());
}
