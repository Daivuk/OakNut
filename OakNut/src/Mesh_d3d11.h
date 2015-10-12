#if defined(ONUT_RENDERER_D3D11)
#pragma once
#include "Mesh.h"

#include <d3d11.h>

namespace onut
{
    class Mesh_d3d11 : public Mesh
    {
    public:
        bool setData(float* vertices, const std::vector<eElement>& elements, uint32_t vertexCount, uint16_t* indices, uint32_t indexCount) override;
        ID3D11Buffer* getVertexBuffer() const;
        ID3D11Buffer* getIndexBuffer() const;
        ID3D11InputLayout* getInputLayout() const;
        ID3D11VertexShader* getVertexShader() const;
        ID3D11PixelShader* getPixelShader() const;
        uint32_t getCount() const;
        uint32_t getStride() const;

    protected:
        friend Mesh;
        Mesh_d3d11() {}
        virtual ~Mesh_d3d11();

    private:
        ID3D11Buffer* m_pVertexBuffer = nullptr;
        ID3D11Buffer* m_pIndexBuffer = nullptr;
        uint32_t m_count = 0;
        uint32_t m_stride = 0;

        ID3D11InputLayout* m_pInputLayout = nullptr;
        // I know it might not make sense that the mesh holds a state like shader, but shaders in this
        // engine don't define much. The material is set as a constant buffer and they all go through
        // the "same" effects. What bounds them differently are the vertex layout of the mesh
        ID3D11VertexShader* m_pVertexShader = nullptr;
        ID3D11PixelShader* m_pPixelShader = nullptr;
    };
}

#endif
