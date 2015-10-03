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

    protected:
        IWindow();

    private:
        COMPONENT_PROPERTY(std::string, WindowName, "Unnamed game");
    };
}
