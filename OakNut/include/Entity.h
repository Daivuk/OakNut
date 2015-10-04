#pragma once
#include "ComponentManager.h"
#include "Object.h"

namespace onut
{
    class Entity : 
        public ComponentManager, // It has components
        public onut::Object // It has ref counting
    {
    public:
        virtual ~Entity();

    private:
        PROPERTY(std::string, Name, "Entity");
    };
}
