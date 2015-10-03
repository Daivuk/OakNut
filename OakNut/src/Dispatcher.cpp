#include "Dispatcher.h"

void onut::Dispatcher::onUpdate()
{
    m_synchronous.processQueue();
}
