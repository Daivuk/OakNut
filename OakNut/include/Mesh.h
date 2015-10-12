#pragma once
#include "Resource.h"

namespace onut
{
    class Mesh : public Resource
    {
    public:
        enum class eElement : uint32_t
        {
            POSITION,
            NORMAL,
            TANGENT,
            BINORMAL,
            TEXCOORD,
            COLOR,
            CUSTOM1,
            CUSTOM2,
            CUSTOM3,
            CUSTOM4
        };

        static Mesh* create();
        static uint32_t getStructureSize(const std::vector<eElement>& elements);

        bool load(const std::string& filename);
        virtual bool setData(float* vertices, const std::vector<eElement>& elements, uint32_t vertexCount, uint16_t* indices, uint32_t indexCount) = 0;

    protected:
        Mesh() {}
        virtual ~Mesh() {}
    };
}
