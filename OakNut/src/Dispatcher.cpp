#include "Dispatcher.h"

void onut::Dispatcher::onUpdate(const onut::TimeInfo& timeInfo)
{
    m_synchronous.processQueue();
}
