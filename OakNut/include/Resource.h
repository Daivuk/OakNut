#pragma once
#include "Object.h"
#include "PropertyManager.h"

namespace onut
{
    class Resource
        : public Object // Has ref counting
        , public PropertyManager
    {
    public:
        virtual ~Resource() {}

    private:
        PROPERTY(std::string, Name, "Resource");
    };
}
