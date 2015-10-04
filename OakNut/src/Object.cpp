#include "Object.h"

onut::Object::~Object()
{
}

void onut::Object::retain()
{
    ++m_refCount;
}
 
void onut::Object::release()
{
    --m_refCount;
    if (m_refCount <= 0)
    {
        delete this;
    }
}

int onut::Object::getRefCount() const
{
    return m_refCount;
}
