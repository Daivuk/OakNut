#pragma once
#include "Resource.h"

namespace onut
{
    class Mesh : public Resource
    {
    public:
        struct sVertex
        {
            float x, y, z;
            float nx, ny, nz;
            float u, v;
            float r, g, b;
        };

        static Mesh* create();

        bool load(const std::string& filename);
        virtual bool setData(sVertex* vertices, uint32_t vertexCount, uint16_t* indices, uint32_t indexCount) = 0;

    protected:
        Mesh() {}
        virtual ~Mesh() {}
    };
}
