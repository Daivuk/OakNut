#include "Torch.h"

#include <ComponentManager.h>
#include <PointLight.h>
#include <Random.h>

Torch::~Torch()
{
    if (m_pPointLight) m_pPointLight->release();
}

void Torch::onCreate()
{
    m_pPointLight = getComponent<onut::PointLight>();
    m_pPointLight->retain();
    if (m_pPointLight)
    {
        m_originalColor = m_pPointLight->getColor();
        m_targetBrightness = m_brightness = m_originalColor.a;
    }
}

void Torch::onUpdate(const onut::TimeInfo& timeInfo)
{
    if (!m_pPointLight) return;

    m_progress -= timeInfo.fDeltaTime;
    if (m_progress <= 0)
    {
        m_progress += getSpeed();
        m_brightness = m_targetBrightness;
        m_targetBrightness = onut::randf(m_originalColor.a - getAmount(), m_originalColor.a + getAmount());
        m_targetBrightness = glm::max(m_targetBrightness, 0.f);
    }

    float brightness = glm::mix(m_brightness, m_targetBrightness, 1.f - m_progress / getSpeed());
    m_pPointLight->setColor(glm::vec4(m_originalColor.r, m_originalColor.g, m_originalColor.b, brightness));
}
