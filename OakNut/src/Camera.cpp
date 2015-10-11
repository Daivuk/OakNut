#include "Camera.h"
#include "Entity.h"
#include "Game.h"
#include "glm/gtc/matrix_transform.hpp"
#include "MeshRenderer.h"
#include "Renderer.h"
#include "Window.h"

void onut::Camera::onUpdate(const onut::TimeInfo& timeInfo)
{
    if (getActive())
    {
        auto pRenderer = onut::Game::getGame()->getComponent<Renderer>();
        pRenderer->setCamera(this);
    }
}

glm::mat4 onut::Camera::getViewProj() const
{
    glm::mat4 ret(1);

    if (!getComponentManager()) return ret;
    auto pEntity = dynamic_cast<Entity*>(getComponentManager());
    if (!pEntity) return ret;

    auto pWindow = onut::Game::getGame()->getComponent<Window>();

    const glm::mat4& world = pEntity->getWorldMatrix();

    auto projection = glm::perspectiveFovRH(glm::radians(getFov()),
                                            static_cast<float>(pWindow->getRealResolution().x),
                                            static_cast<float>(pWindow->getRealResolution().y),
                                            getNear(), getFar());
    glm::vec3 eye{world[3]};
    glm::vec3 center{world[3] + world[1]};
    glm::vec3 up{world[2]};
    auto view = glm::lookAtRH(eye, center, up);

    ret = projection * view;

    return std::move(ret);
}
