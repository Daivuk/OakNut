#if defined(ONUT_RENDERER_D3D11)
#include "Mesh_d3d11.h"

onut::Mesh* onut::Mesh::create()
{
    return new Mesh_d3d11();
}

onut::Mesh_d3d11::~Mesh_d3d11()
{
}

#endif
