#pragma once
#include "Object.h"
#include "PropertyManager.h"

namespace onut
{
    class Resource :
        public PropertyManager, // Has serializable properties
        public Object // Has ref counting
    {
    public:
        virtual ~Resource() {}
    private:
    };
}
