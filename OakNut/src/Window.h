#pragma once
#include "Component.h"

namespace onut
{
    class Window : public Component
    {
    public:
        Window();
        virtual ~Window();

        virtual void onCreate() = 0;
        virtual void onUpdate(const onut::TimeInfo& timeInfo) = 0;

        const glm::tvec2<int>& getRealResolution() const;

        PROPERTY(std::string, Caption, "Unamed Game");
        PROPERTY(glm::tvec2<int>, Resolution, glm::tvec2<int>(1280, 720));

    protected:
        glm::tvec2<int> m_realResolution;
    };
}
