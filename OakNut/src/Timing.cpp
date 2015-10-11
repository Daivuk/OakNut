#include "Timing.h"

onut::Timing::Timing()
{
}

void onut::Timing::onCreate()
{
    m_lastTime = std::chrono::high_resolution_clock::now();
}

void onut::Timing::onUpdate(const onut::TimeInfo& timeInfo)
{
    auto now = std::chrono::high_resolution_clock::now();
    m_deltaTime = now - m_lastTime;
    m_lastTime = now;

    m_fDeltaTime = static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(m_deltaTime).count()) / 1000.0f;
    m_timeInfo.fDeltaTime = m_fDeltaTime;
}

void onut::Timing::onDraw()
{
    auto now = std::chrono::high_resolution_clock::now();
    if (now - m_fpsTime >= std::chrono::seconds(1))
    {
        m_fpsTime = now;
        m_fps = m_fpsCounter;
        m_fpsCounter = 0;
    }
    else
    {
        ++m_fpsCounter;
    }
}

float onut::Timing::getDeltaTime() const
{
    return m_fDeltaTime;
}

const onut::TimeInfo& onut::Timing::getTimeInfo() const
{
    return m_timeInfo;
}
