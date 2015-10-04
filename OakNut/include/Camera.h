#pragma once
#include "Component.h"

namespace onut
{
    class Camera : public Component
    {
    public:
        glm::mat4 getViewProj() const;

    private:
        PROPERTY(bool, Orthographic, false);
        PROPERTY(float, Fov, 70.f);
        PROPERTY(float, Near, 0.1f);
        PROPERTY(float, Far, 10000.f);
    };
}
