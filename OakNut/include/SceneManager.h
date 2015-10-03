#pragma once
#include "Component.h"

namespace onut
{
    class SceneNode;

    class SceneManager : public Component
    {
    public:
        SceneManager();
        virtual ~SceneManager();

        void loadScene(const std::string& filename);

        void onCreate() override;
        void onUpdate() override;
        void onDraw() override;

    private:
        SceneNode *m_pRoot = nullptr;
    };
}
