#pragma once
#include "Entity.h"
#include "glm/glm.hpp"

namespace onut
{
    class Dispatcher;
    class IRenderer;
    class IWindow;
    class SceneManager;
    class Timing;

    class Game : 
        public onut::ComponentManager, // Has components "services"
        public onut::PropertyManager, // Has serializable properties
        public onut::Object // Has ref counting
    {
    public:
        static Game* CreateGame();

        static Game* getGame();
        static Dispatcher* getMainDispatcher();
        static IRenderer* getRenderer();
        static SceneManager* getSceneManager();
        static Timing* getTiming();
        static IWindow* getWindow();

        PROPERTY(std::string, Name, "Unamed Game");
        PROPERTY(glm::tvec2<int>, Resolution, glm::tvec2<int>(1280, 720));

    public:
        virtual ~Game();

        virtual void onLoaded() {}

    private:
        friend class Main;

        static Game* s_pGame;
        static Dispatcher* s_pDispatcher;
        static IRenderer* s_pRenderer;
        static SceneManager* s_pSceneManager;
        static Timing* s_pTiming;
        static IWindow* s_pWindow;
    };
}
