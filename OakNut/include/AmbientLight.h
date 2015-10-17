#pragma once
#include "Component.h"

namespace onut
{
    class AmbientLight : public Component
    {
        PROPERTY(glm::vec4, Color, glm::vec4(1, 1, 1, .25f));
    };
}
