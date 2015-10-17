#pragma once
#include <unordered_map>
#include <string>

#define REG_COMPONENT(__class__) onut::ObjectLibrary::registerObject<__class__>(#__class__)
#define REG_COMPONENT2(__class__, __name__) onut::ObjectLibrary::registerObject<__class__>(#__name__)

namespace onut
{
    class Object;
    class ObjectLibrary
    {
    public:
        static void clear();

        static onut::Object* createObject(const std::string& name);
        template<typename Tobject> static bool registerObject(const std::string& name);

    private:
        ObjectLibrary() {}

        class IObjectFactory
        {
        public:
            virtual onut::Object* createObject() const = 0;
        };
        template<typename Tobject>
        class ObjectFactory : public IObjectFactory
        {
        public:
            onut::Object* createObject() const
            {
                return new Tobject();
            }
        };
        static std::unordered_map<std::string, IObjectFactory*> m_objectFactories;
    };

    template<typename Tobject>
    inline bool ObjectLibrary::registerObject(const std::string& name)
    {
        auto it = m_objectFactories.find(name);
        if (it != m_objectFactories.end())
        {
            return false;
        }
        m_objectFactories[name] = new ObjectFactory<Tobject>();
        return true;
    }
}
