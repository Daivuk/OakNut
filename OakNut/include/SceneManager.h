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
        void loadSceneAsync(const std::string& filename);

        void onCreate() override;
        void onUpdate() override;
        void onDraw() override;

        PROPERTY(std::string, StartupScene, "");

    private:
        void createEntity(Entity* pEntity);
        void updateEntity(Entity* pEntity);
        void drawEntity(Entity* pEntity);
        void removeNonPersist(Entity* pEntity);

        Entity *m_pRootEntity = nullptr;
    };
}
