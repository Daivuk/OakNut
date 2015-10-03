#pragma once

#include <cassert>
#include <string>
#include <unordered_map>

namespace onut
{
#define PROPERTY(__type__, __name__, __defaultValue__) \
    private: \
        __type__ m_ ## __name__ = __defaultValue__; \
        bool m_ ## __name__ ## IsDefault = registerProperty(#__name__, &m_ ## __name__); \
    public: \
        const __type__& get ## __name__() const {return m_ ## __name__;} \
        void set ## __name__(const __type__& value) \
        { \
            m_ ## __name__ ## IsDefault = value == __defaultValue__; \
            m_ ## __name__ = value; \
        }

    class PropertyManager
    {
    public:
        virtual ~PropertyManager();

    protected:
        template<typename Ttype> bool registerProperty(const std::string& name, Ttype* pVar);

    private:
        std::unordered_map<std::string, void*> m_properties;
    };

    template<typename Ttype>
    inline bool PropertyManager::registerProperty(const std::string& name, Ttype* pVar)
    {
        // Assert if dupilcated property.
        // You added a property using PROPERTY macro, but a
        // property with that name already exist in this component.
        assert(m_properties.find(name) == m_properties.end());

        m_properties[name] = pVar;
        return true;
    }
}
