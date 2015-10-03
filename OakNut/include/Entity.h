#pragma once
#include "ComponentManager.h"
#include "Object.h"

#include <assert.h>
#include <unordered_map>

namespace onut
{
#define ENTITY_PROPERTY(__type__, __name__, __defaultValue__) \
    private: \
        __type__ m_ ## __name__ = __defaultValue__; \
        bool m_ ## __name__ ## Registered = registerProperty(#__name__, &m_ ## __name__); \
    public: \
        const __type__& get ## __name__() const {return m_ ## __name__;} \
        void set ## __name__(const __type__& value) \
        { \
            m_ ## __name__ = value; \
        }

    class Entity : public ComponentManager, public onut::Object
    {
    public:
        virtual ~Entity();

    protected:
        template<typename Ttype> bool registerProperty(const std::string& name, Ttype* pVar);

    private:
        std::unordered_map<std::string, void*> m_properties;

        // Common component properties
        ENTITY_PROPERTY(std::string, Name, "Entity");
    };

    template<typename Ttype>
    inline bool Entity::registerProperty(const std::string& name, Ttype* pVar)
    {
        // Assert if dupilcated property.
        // You added a property using ENTITY_PROPERTY macro, but a
        // property with that name already exist in this entity.
        assert(m_properties.find(name) == m_properties.end());

        m_properties[name] = pVar;
        return true;
    }
}
