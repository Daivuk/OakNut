#pragma once
#include "Component.h"

namespace onut
{
    class IWindow : public Component
    {
    public:
        static IWindow* createWindow();
        virtual ~IWindow();

        virtual void onCreate() = 0;
        virtual void onUpdate() = 0;

        const glm::tvec2<int>& getRealResolution() const;

        PROPERTY(std::string, Caption, "Unamed Game");
        PROPERTY(glm::tvec2<int>, Resolution, glm::tvec2<int>(1280, 720));

    protected:
        IWindow();

        glm::tvec2<int> m_realResolution;
    };
}
