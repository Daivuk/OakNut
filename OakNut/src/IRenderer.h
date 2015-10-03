#pragma once
#include "Component.h"

namespace onut
{
    class IWindow;

    class IRenderer : public Component
    {
    public:
        static IRenderer* createRenderer(IWindow* pWindow);
        virtual ~IRenderer();

        virtual void onCreate() = 0;
        virtual void onDraw() = 0;

    protected:
        IRenderer();
    };
}
