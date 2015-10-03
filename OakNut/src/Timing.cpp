#include "Timing.h"

onut::Timing::Timing()
{
    setName("Timing");
}

void onut::Timing::onCreate()
{
    m_lastTime = std::chrono::high_resolution_clock::now();
}

void onut::Timing::onUpdate()
{
    auto now = std::chrono::high_resolution_clock::now();
    m_deltaTime = now - m_lastTime;
    m_lastTime = now;

    m_fDeltaTime = static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(m_deltaTime).count()) / 1000.0f;
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
