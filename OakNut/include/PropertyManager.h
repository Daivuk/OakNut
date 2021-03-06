#pragma once
#include "ObjectVector.h"

#include "glm/glm.hpp"
#include "json/json.h"

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
#define PROPERTY_DIRTY(__type__, __name__, __defaultValue__, __dirtyFunc__) \
    private: \
        __type__ m_ ## __name__ = __defaultValue__; \
        bool m_ ## __name__ ## IsDefault = registerProperty(#__name__, &m_ ## __name__); \
    public: \
        const __type__& get ## __name__() const {return m_ ## __name__;} \
        void set ## __name__(const __type__& value) \
        { \
            m_ ## __name__ ## IsDefault = value == __defaultValue__; \
            m_ ## __name__ = value; \
            __dirtyFunc__(this); \
        }
#define PROPERTY_OBJECT(__type__, __name__, __defaultValue__) \
    private: \
        __type__* m_p ## __name__ = __defaultValue__; \
        bool m_ ## __name__ ## IsDefault = registerProperty(#__name__, &m_p ## __name__); \
        struct s ## __name__ ## Destructor \
        { \
            s ## __name__ ## Destructor(__type__** ppIn ## __name__) \
                : pp ## __name__(ppIn ## __name__) {} \
            virtual ~s ## __name__ ## Destructor() \
            { \
                if (*pp ## __name__) (*pp ## __name__)->release(); \
            } \
            __type__** pp ## __name__; \
        }; \
        s ## __name__ ## Destructor m_ ## __name__ ## Destructor = s ## __name__ ## Destructor(&m_p ## __name__); \
    public: \
        __type__* get ## __name__() const {return m_p ## __name__;} \
        void set ## __name__(__type__* pValue) \
        { \
            m_ ## __name__ ## IsDefault = pValue == __defaultValue__; \
            if (pValue) pValue->retain(); \
            if (m_p ## __name__) m_p ## __name__->release(); \
            m_p ## __name__ = pValue; \
        }

#define PROPERTY_NOSETTER(__type__, __name__, __defaultValue__) \
    private: \
        __type__ m_ ## __name__ = __defaultValue__; \
        bool m_ ## __name__ ## IsDefault = registerProperty(#__name__, &m_ ## __name__); \
    public: \
        const __type__& get ## __name__() const {return m_ ## __name__;}

    class Component;
    class Entity;
    class Material;
    class Mesh;
    class Object;
    class Texture;

    class PropertyManager
    {
    public:
        virtual ~PropertyManager();

        bool loadPropertiesFromFile(const std::string& filename);
        bool loadPropertiesFromJson(const Json::Value& json);

        bool removeWeakReferences(onut::Object* pObjectToDereference);

    protected:
        template<typename Ttype> bool registerProperty(const std::string& name, Ttype* pVar);

    private:
        enum class ePropertyType
        {
            P_UNKNOWN,
            P_BOOL,
            P_INT,
            P_FLOAT,
            P_STRING,
            P_POINT,
            P_VEC2,
            P_VEC3,
            P_VEC4,
            P_ENTITY,
            P_ENTITY_ARRAY,
            P_COMPONENT_ARRAY,
            P_MATERIAL,
            P_MESH,
            P_TEXTURE,
        };
        struct SPropertyLink
        {
            ePropertyType type;
            void* pProperty;
        };
        static SPropertyLink make_property(bool *pVar) { return{ePropertyType::P_BOOL, pVar}; }
        static SPropertyLink make_property(int *pVar) { return{ePropertyType::P_INT, pVar}; }
        static SPropertyLink make_property(float *pVar) { return{ePropertyType::P_FLOAT, pVar}; }
        static SPropertyLink make_property(std::string *pVar) { return{ePropertyType::P_STRING, pVar}; }
        static SPropertyLink make_property(glm::tvec2<int> *pVar) { return{ePropertyType::P_POINT, pVar}; }
        static SPropertyLink make_property(glm::vec2 *pVar) { return{ePropertyType::P_VEC2, pVar}; }
        static SPropertyLink make_property(glm::vec3 *pVar) { return{ePropertyType::P_VEC3, pVar}; }
        static SPropertyLink make_property(glm::vec4 *pVar) { return{ePropertyType::P_VEC4, pVar}; }
        static SPropertyLink make_property(Entity **pVar) { return{ePropertyType::P_ENTITY, pVar}; }
        static SPropertyLink make_property(ObjectVector<Entity> *pVar) { return{ePropertyType::P_ENTITY_ARRAY, pVar}; }
        static SPropertyLink make_property(ObjectVector<Component> *pVar) { return{ePropertyType::P_COMPONENT_ARRAY, pVar}; }
        static SPropertyLink make_property(Material **pVar) { return{ePropertyType::P_MATERIAL, (void*)pVar}; }
        static SPropertyLink make_property(Mesh **pVar) { return{ePropertyType::P_MESH, (void*)pVar}; }
        static SPropertyLink make_property(Texture **pVar) { return{ePropertyType::P_TEXTURE, (void*)pVar}; }
        std::unordered_map<std::string, SPropertyLink> m_properties;
    };

    template<typename Ttype>
    inline bool PropertyManager::registerProperty(const std::string& name, Ttype* pVar)
    {
        // Assert if dupilcated property.
        // You added a property using PROPERTY macro, but a
        // property with that name already exist in this component.
        assert(m_properties.find(name) == m_properties.end());

        m_properties[name] = make_property(pVar);
        return true;
    }
}
