#include "Camera.h"
#include "Game.h"
#include "IRenderer.h"
#include "MeshRenderer.h"
#include "SceneNode.h"

glm::mat4 onut::Camera::getViewProj() const
{
    glm::mat4 ret(1);

    if (!getComponentManager()) return ret;
    auto pSceneNode = getComponentManager()->getComponent<SceneNode>();
    if (!pSceneNode) return ret;

    const glm::mat4& world = pSceneNode->getWorldMatrix();

    return std::move(ret);
}

    //private:
    //    PROPERTY(bool, Orthographic, false);
    //    PROPERTY(float, Fov, 70.f);
    //    PROPERTY(float, Near, 0.1f);
    //    PROPERTY(float, Far, 10000.f);
