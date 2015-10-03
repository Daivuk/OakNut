#pragma once
#include "ComponentManager.h"
#include "Object.h"

namespace onut
{
    class IGame : public onut::ComponentManager, public onut::Object
    {
    public:
        static IGame* CreateGame();

    public:
        virtual ~IGame();
    };
}
