#pragma once
#include <Component.h>

struct sRoom
{
    int32_t w;
    int32_t h;
    char* tiles;
};

class LevelGenerator : public onut::Component
{
public:
    void onCreate() override;

protected:
    static onut::Entity* createRoom(int startX, int startY, const sRoom& room);
    static onut::Entity* createTorch(const glm::vec2& position);
};
