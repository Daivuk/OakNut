#pragma once
#include "Object.h"

#include <string>
#include <unordered_map>

namespace onut
{
#define COMPONENT_PROPERTY(__type__, __name__, __defaultValue__) \
    private: \
        __type__ m_ ## __name__ = __defaultValue__; \
        bool m_ ## __name__ ## Registered_ ## __LINE__ = registerProperty(#__name__, &m_ ## __name__); \
    public: \
        const __type__& get ## __name__() const {return m_ ## __name__;} \
        void set ## __name__(const __type__& value) \
        { \
            m_ ## __name__ = value; \
        }

    class Component : public Object
    {
    public:
        Component();
        virtual ~Component();

        bool isCreated() const;

        // Component events
        virtual void onCreate() {}
        virtual void onUpdate() {}
        virtual void onDraw() {}

    protected:
        template<typename Ttype>
        bool registerProperty(const std::string& name, Ttype* pVar)
        {
            m_properties[name] = pVar;
            return true;
        }

    private:
        friend class ComponentManager;

        bool m_created = false;
        ComponentManager* m_pComponentManager = nullptr;

        std::unordered_map<std::string, void*> m_properties;

        // Common component properties
        COMPONENT_PROPERTY(bool, Enabled, true);
        COMPONENT_PROPERTY(bool, Visible, true);
        COMPONENT_PROPERTY(std::string, Name, "");
    };
}
