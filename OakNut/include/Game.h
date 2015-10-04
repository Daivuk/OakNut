#pragma once
#include "Entity.h"
#include "glm/glm.hpp"

namespace onut
{
    class Game : 
        public onut::ComponentManager, // Has components "services"
        public onut::Object // Has ref counting
    {
    public:
        Game();

        static Game* CreateGame();
        static Game* getGame();

        PROPERTY(std::string, Filename, "assets/game.json");

    public:
        virtual ~Game();

    private:
        static Game* s_pGame;
    };
}
