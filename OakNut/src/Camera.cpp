#include "Camera.h"
#include "Entity.h"
#include "Game.h"
#include "IRenderer.h"
#include "MeshRenderer.h"

glm::mat4 onut::Camera::getViewProj() const
{
    glm::mat4 ret(1);

    if (!getComponentManager()) return ret;
    auto pEntity = dynamic_cast<Entity*>(getComponentManager());
    if (!pEntity) return ret;

    const glm::mat4& world = pEntity->getWorldMatrix();

    return std::move(ret);
}

    //private:
    //    PROPERTY(bool, Orthographic, false);
    //    PROPERTY(float, Fov, 70.f);
    //    PROPERTY(float, Near, 0.1f);
    //    PROPERTY(float, Far, 10000.f);
