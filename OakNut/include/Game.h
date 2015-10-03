#pragma once
#include "Entity.h"

namespace onut
{
    class Game : public onut::Entity
    {
    public:
        static Game* CreateGame();

    public:
        virtual ~Game();
    };
}
