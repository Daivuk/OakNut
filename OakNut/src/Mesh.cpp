#include "Mesh.h"

#include <fstream>

bool onut::Mesh::load(const std::string& filename)
{
    std::ifstream fic(filename);
    if (fic.fail()) return false;

    unsigned long version;
    try
    {
        fic.read((char*)&version, 4);
    }
    catch (std::exception e)
    {
        fic.close();
        return false;
    }
    
    switch (version)
    {
        case 1:
        {
            sVertex* vertices = nullptr;
            uint16_t* indices = nullptr;

            try
            {
                uint32_t nbVertices;
                uint32_t nbIndices;

                fic.read((char*)&nbVertices, 4);
                fic.read((char*)&nbIndices, 4);

                if (!nbVertices || !nbIndices)
                {
                    fic.close();
                    return false;
                }

                vertices = new sVertex[nbVertices];
                indices = new uint16_t[nbIndices];

                fic.read((char*)vertices, nbVertices * sizeof(sVertex));
                fic.read((char*)indices, nbIndices * sizeof(uint16_t));
                fic.close();

                setData(vertices, nbVertices, indices, nbIndices);
            }
            catch (std::exception e)
            {
                if (vertices) delete[] vertices;
                if (indices) delete[] indices;
                fic.close();
                return false;
            }
            return true;
        }
        default:
        {
            fic.close();
            return false;
        }
    }

    return false;
}
