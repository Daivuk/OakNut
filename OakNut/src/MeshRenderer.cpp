#include "Game.h"
#include "IRenderer.h"
#include "MeshRenderer.h"
#include "SceneNode.h"

void onut::MeshRenderer::onDraw()
{
    if (!getMesh() || !getMaterial() || !getComponentManager()) return;
    auto pSceneNode = getComponentManager()->getComponent<SceneNode>();
    if (!pSceneNode) return;

    Game::getGame()->getComponent<IRenderer>()->draw(getMesh(), getMaterial(), pSceneNode->getWorldMatrix());
}
