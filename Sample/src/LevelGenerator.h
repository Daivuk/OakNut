#pragma once
#include <Component.h>

class LevelGenerator : public onut::Component
{
public:
    void onCreate() override;
    static onut::Entity* createTorch(const glm::vec2& position);
};
