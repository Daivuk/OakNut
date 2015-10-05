#include "Component.h"
#include "ComponentManager.h"
#include "Entity.h"
#include "ObjectLibrary.h"
#include "PropertyManager.h"

#include <fstream>

onut::PropertyManager::~PropertyManager()
{
}

bool onut::PropertyManager::loadPropertiesFromFile(const std::string& filename)
{
    std::ifstream fic(filename);
    if (fic.fail()) return false;

    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(fic, root)) return false;

    fic.close();

    return loadPropertiesFromJson(root);
}

bool onut::PropertyManager::loadPropertiesFromJson(const Json::Value& json)
{
    for (auto &member : json.getMemberNames())
    {
        auto propertyIT = m_properties.find(member);
        if (propertyIT == m_properties.end()) continue;

        auto &propertyLink = propertyIT->second;
        auto &jsonElement = json[member];

        switch (propertyLink.type)
        {
            case ePropertyType::P_BOOL:
                if (jsonElement.isBool())
                {
                    *(bool*)propertyLink.pProperty = jsonElement.asBool();
                }
                break;
            case ePropertyType::P_INT:
                if (jsonElement.isInt())
                {
                    *(int*)propertyLink.pProperty = jsonElement.asInt();
                }
                break;
            case ePropertyType::P_FLOAT:
                if (jsonElement.isDouble())
                {
                    *(float*)propertyLink.pProperty = static_cast<float>(jsonElement.asDouble());
                }
                break;
            case ePropertyType::P_STRING:
                if (jsonElement.isString())
                {
                    *(std::string*)propertyLink.pProperty = jsonElement.asCString();
                }
                break;
            case ePropertyType::P_POINT:
                if (jsonElement.isArray() &&
                    jsonElement.size() == 2)
                {
                    auto &x = jsonElement[0];
                    auto &y = jsonElement[1];
                    if (x.isInt() && y.isInt())
                    {
                        ((glm::tvec2<int>*)propertyLink.pProperty)->x = x.asInt();
                        ((glm::tvec2<int>*)propertyLink.pProperty)->y = y.asInt();
                    }
                }
                break;
            case ePropertyType::P_VEC2:
                if (jsonElement.isArray() &&
                    jsonElement.size() == 2)
                {
                    auto &x = jsonElement[0];
                    auto &y = jsonElement[1];
                    if (x.isDouble() && y.isDouble())
                    {
                        ((glm::vec2*)propertyLink.pProperty)->x = static_cast<float>(x.asDouble());
                        ((glm::vec2*)propertyLink.pProperty)->y = static_cast<float>(y.asDouble());
                    }
                }
                break;
            case ePropertyType::P_VEC3:
                if (jsonElement.isArray() &&
                    jsonElement.size() == 3)
                {
                    auto &x = jsonElement[0];
                    auto &y = jsonElement[1];
                    auto &z = jsonElement[2];
                    if (x.isDouble() && y.isDouble() && z.isDouble())
                    {
                        ((glm::vec3*)propertyLink.pProperty)->x = static_cast<float>(x.asDouble());
                        ((glm::vec3*)propertyLink.pProperty)->y = static_cast<float>(y.asDouble());
                        ((glm::vec3*)propertyLink.pProperty)->z = static_cast<float>(z.asDouble());
                    }
                }
                break;
            case ePropertyType::P_VEC4:
                if (jsonElement.isArray() &&
                    jsonElement.size() == 4)
                {
                    auto &x = jsonElement[0];
                    auto &y = jsonElement[1];
                    auto &z = jsonElement[2];
                    auto &w = jsonElement[3];
                    if (x.isDouble() && y.isDouble() && z.isDouble() && w.isDouble())
                    {
                        ((glm::vec4*)propertyLink.pProperty)->x = static_cast<float>(x.asDouble());
                        ((glm::vec4*)propertyLink.pProperty)->y = static_cast<float>(y.asDouble());
                        ((glm::vec4*)propertyLink.pProperty)->z = static_cast<float>(z.asDouble());
                        ((glm::vec4*)propertyLink.pProperty)->w = static_cast<float>(w.asDouble());
                    }
                }
                break;
            case ePropertyType::P_ENTITY_ARRAY:
                if (jsonElement.isArray())
                {
                    auto pMyEntity = dynamic_cast<Entity*>(this);
                    auto pEntities = static_cast<std::vector<onut::Entity*>*>(propertyLink.pProperty);
                    for (auto &jsonEntity : jsonElement)
                    {
                        if (jsonEntity.isObject())
                        {
                            if (pMyEntity)
                            {
                                auto pEntity = new onut::Entity();
                                pEntity->retain();
                                pEntity->loadPropertiesFromJson(jsonEntity);
                                pMyEntity->add(pEntity);
                                pEntity->release();
                            }
                            else
                            {
                                assert(false); // TODO
                            }
                        }
                    }
                }
                break;
            case ePropertyType::P_COMPONENT_ARRAY:
                if (jsonElement.isArray())
                {
                    auto pComponentManager = dynamic_cast<ComponentManager*>(this);
                    auto pComponents = static_cast<std::vector<onut::Component*>*>(propertyLink.pProperty);
                    for (auto &jsonComponent : jsonElement)
                    {
                        if (jsonComponent.isArray() &&
                            jsonComponent.size() == 2 &&
                            jsonComponent[0].isString() &&
                            jsonComponent[1].isObject())
                        {
                            auto szComponentClassName = jsonComponent[0].asCString();
                            auto pComponentObject = ObjectLibrary::createObject(szComponentClassName);
                            if (!pComponentObject) continue;
                            auto pComponent = dynamic_cast<onut::Component*>(pComponentObject);
                            if (!pComponent) continue;
                            pComponent->loadPropertiesFromJson(jsonComponent[1]);
                            pComponent->retain();
                            if (pComponentManager)
                            {
                                pComponentManager->addComponent(pComponent);
                                pComponent->release();
                            }
                            else
                            {
                                pComponents->push_back(pComponent);
                            }
                        }
                    }
                }
                break;
        }
    }

    return true;
}
