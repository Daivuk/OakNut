#pragma once
#include "Component.h"

#include <chrono>

namespace onut
{
    class Timing : public Component
    {
    public:
        Timing();

        void onCreate() override;
        void onUpdate() override;
        void onDraw() override;

        float getDeltaTime() const;
        int getFPS() const;

    private:
        std::chrono::high_resolution_clock::time_point m_lastTime;
        std::chrono::high_resolution_clock::duration m_deltaTime;
        std::chrono::high_resolution_clock::time_point m_fpsTime;
        int m_fpsCounter = 0;
        float m_fDeltaTime = 0.f;
        int m_fps = 0;
    };
}
