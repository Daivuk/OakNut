#if defined(ONUT_RENDERER_D3D11)
#pragma once
#include "Mesh.h"

namespace onut
{
    class Mesh_d3d11 : public Mesh
    {
    public:
    protected:
        friend Mesh;
        Mesh_d3d11() {}
        virtual ~Mesh_d3d11();
    };
}

#endif
