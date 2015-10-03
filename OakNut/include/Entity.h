#pragma once
#include "ComponentManager.h"
#include "Object.h"
#include "PropertyManager.h"

namespace onut
{
    class Entity : 
        public ComponentManager, // It has components
        public PropertyManager, // It has serializable properties
        public onut::Object // It has ref counting
    {
    public:
        virtual ~Entity();

    private:
        PROPERTY(std::string, Name, "Entity");
    };
}
