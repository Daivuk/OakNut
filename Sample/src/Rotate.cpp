#include "Entity.h"
#include "Rotate.h"

void Rotate::onUpdate(const onut::TimeInfo& timeInfo)
{
    auto rotation = getEntity()->getRotation();
    rotation.z += getSpeed() * timeInfo.fDeltaTime;
    getEntity()->setRotation(rotation);
}
