#pragma once
#include "Component.h"
#include "TimeInfo.h"

#include <chrono>

namespace onut
{
    class Timing : public Component
    {
    public:
        Timing();

        void onCreate() override;
        void onUpdate(const onut::TimeInfo& timeInfo) override;
        void onDraw() override;

        float getDeltaTime() const;
        int getFPS() const;
        const TimeInfo& getTimeInfo() const;

    private:
        std::chrono::high_resolution_clock::time_point m_lastTime;
        std::chrono::high_resolution_clock::duration m_deltaTime;
        std::chrono::high_resolution_clock::time_point m_fpsTime;
        int m_fpsCounter = 0;
        float m_fDeltaTime = 0.f;
        int m_fps = 0;
        TimeInfo m_timeInfo;
    };
}
