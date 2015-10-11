#pragma once
#include <Component.h>

class Rotate : public onut::Component
{
public:
    void onUpdate(const onut::TimeInfo& timeInfo) override;

    PROPERTY(float, Speed, 10.f);
};
