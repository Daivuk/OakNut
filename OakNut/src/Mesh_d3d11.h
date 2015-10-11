#if defined(ONUT_RENDERER_D3D11)
#pragma once
#include "Mesh.h"

#include <d3d11.h>

namespace onut
{
    class Mesh_d3d11 : public Mesh
    {
    public:
        bool setData(sVertex* vertices, uint32_t vertexCount, uint16_t* indices, uint32_t indexCount) override;
        ID3D11Buffer* getVertexBuffer() const;
        ID3D11Buffer* getIndexBuffer() const;
        uint32_t getCount() const;

    protected:
        friend Mesh;
        Mesh_d3d11() {}
        virtual ~Mesh_d3d11();

    private:
        ID3D11Buffer* m_pVertexBuffer = nullptr;
        ID3D11Buffer* m_pIndexBuffer = nullptr;
        uint32_t m_count = 0;
    };
}

#endif
