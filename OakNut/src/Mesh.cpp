#include "Mesh.h"

#include <stdio.h>

uint32_t onut::Mesh::getStructureSize(const std::vector<eElement>& elements)
{
    uint32_t structureSize = 0;
    for (auto& element : elements)
    {
        switch (element)
        {
            case eElement::POSITION:
                structureSize += 3;
                break;
            case eElement::NORMAL:
                structureSize += 3;
                break;
            case eElement::TANGENT:
                structureSize += 3;
                break;
            case eElement::BINORMAL:
                structureSize += 3;
                break;
            case eElement::TEXCOORD:
                structureSize += 2;
                break;
            case eElement::COLOR:
                structureSize += 3;
                break;
            case eElement::CUSTOM1:
                structureSize += 1;
                break;
            case eElement::CUSTOM2:
                structureSize += 2;
                break;
            case eElement::CUSTOM3:
                structureSize += 3;
                break;
            case eElement::CUSTOM4:
                structureSize += 4;
                break;
        }
    }
    return structureSize;
}

bool onut::Mesh::load(const std::string& filename)
{
    FILE* pFic = nullptr;
#if defined(WIN32)
    fopen_s(&pFic, filename.c_str(), "rb");
    if (!pFic) return false;
#else
#error Unimplemented
#endif

    uint32_t version;
    fread(&version, sizeof(uint32_t), 1, pFic);
    
    switch (version)
    {
        case 2:
        {
            const uint32_t FLAG_HAS_COLOR = 0x1;
            const uint32_t FLAG_HAS_BINORMALS = 0x2;

            uint32_t flags;
            fread(&flags, sizeof(uint32_t), 1, pFic);
            std::vector<eElement> elements;
            switch (flags)
            {
                case 0:
                    elements = {eElement::POSITION, eElement::NORMAL, eElement::TEXCOORD};
                    break;
                case FLAG_HAS_COLOR:
                    elements = {eElement::POSITION, eElement::NORMAL, eElement::TEXCOORD, eElement::COLOR};
                    break;
                case FLAG_HAS_BINORMALS:
                    elements = {eElement::POSITION, eElement::NORMAL, eElement::TANGENT, eElement::BINORMAL, eElement::TEXCOORD};
                    break;
                case FLAG_HAS_COLOR | FLAG_HAS_BINORMALS:
                    elements = {eElement::POSITION, eElement::NORMAL, eElement::TANGENT, eElement::BINORMAL, eElement::TEXCOORD, eElement::COLOR};
                    break;
                default:
                {
                    fclose(pFic);
                    return false;
                }
            }

            float* vertices = nullptr;
            uint16_t* indices = nullptr;

            uint32_t nbVertices;
            uint32_t nbIndices;
            auto structureSize = getStructureSize(elements);

            fread(&nbVertices, sizeof(uint32_t), 1, pFic);
            fread(&nbIndices, sizeof(uint32_t), 1, pFic);

            if (!nbVertices || !nbIndices)
            {
                fclose(pFic);
                return false;
            }

            vertices = new float[nbVertices * structureSize];
            indices = new uint16_t[nbIndices];

            fread(vertices, sizeof(float), nbVertices * structureSize, pFic);
            fread(indices, sizeof(uint16_t), nbIndices, pFic);
            fclose(pFic);

            setData(vertices, elements, nbVertices, indices, nbIndices);

            delete[] vertices;
            delete[] indices;

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
