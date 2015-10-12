#pragma once
#include "Component.h"

namespace onut
{
    class PointLight : public Component
    {
        PROPERTY(float, Radius, 5.f);
        PROPERTY(glm::vec4, Color, glm::vec4(1, 1, 1, 1));
    };
};
