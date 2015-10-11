#include "Mesh.h"

#include <stdio.h>

bool onut::Mesh::load(const std::string& filename)
{
    FILE* pFic = nullptr;
#if defined(WIN32)
    fopen_s(&pFic, filename.c_str(), "rb");
    if (!pFic) return false;
#else
#error Unimplemented
#endif

    unsigned long version;
    fread(&version, 4, 1, pFic);
    
    switch (version)
    {
        case 1:
        {
            sVertex* vertices = nullptr;
            uint16_t* indices = nullptr;

            uint32_t nbVertices;
            uint32_t nbIndices;

            fread(&nbVertices, sizeof(uint32_t), 1, pFic);
            fread(&nbIndices, sizeof(uint32_t), 1, pFic);

            if (!nbVertices || !nbIndices)
            {
                fclose(pFic);
                return false;
            }

            vertices = new sVertex[nbVertices];
            indices = new uint16_t[nbIndices];

            fread(vertices, sizeof(float), nbVertices * (sizeof(sVertex) / sizeof(float)), pFic);
            fread(indices, sizeof(uint16_t), nbIndices, pFic);
            fclose(pFic);

            setData(vertices, nbVertices, indices, nbIndices);
            return true;
        }
        default:
        {
            fclose(pFic);
            return false;
        }
    }

    return false;
}
