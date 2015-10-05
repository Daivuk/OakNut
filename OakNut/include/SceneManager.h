#pragma once
#include "Component.h"

namespace onut
{
    class Entity;

    class SceneManager : public Component
    {
    public:
        SceneManager();
        virtual ~SceneManager();

        void loadScene(const std::string& filename);

        void onCreate() override;
        void onUpdate() override;
        void onDraw() override;

        PROPERTY(std::string, StartupScene, "");

    private:
        Entity *m_pRootEntity = nullptr;
    };
}
