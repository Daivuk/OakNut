#pragma once
#include <Component.h>
#include <PointLight.h>

class Torch : public onut::Component
{
public:
    ~Torch();

    void onCreate() override;
    void onUpdate(const onut::TimeInfo& timeInfo) override;

    PROPERTY(float, Speed, .15f);
    PROPERTY(float, Amount, .15f);
    
private:
    glm::vec4 m_originalColor;
    onut::PointLight* m_pPointLight = nullptr;
    float m_brightness = 0.f;
    float m_targetBrightness = 0.f;
    float m_progress = 0.f;
};
