#if defined(ONUT_RENDERER_D3D11)
#include "Game.h"
#include "Mesh_d3d11.h"
#include "Renderer_d3d11.h"

onut::Mesh* onut::Mesh::create()
{
    return new Mesh_d3d11();
}

onut::Mesh_d3d11::~Mesh_d3d11()
{
    if (m_pVertexBuffer) m_pVertexBuffer->Release();
    if (m_pIndexBuffer) m_pIndexBuffer->Release();
}

ID3D11Buffer* onut::Mesh_d3d11::getVertexBuffer() const
{
    return m_pVertexBuffer;
}

ID3D11Buffer* onut::Mesh_d3d11::getIndexBuffer() const
{
    return m_pIndexBuffer;
}

uint32_t onut::Mesh_d3d11::getCount() const
{
    return m_count;
}

bool onut::Mesh_d3d11::setData(sVertex* vertices, uint32_t vertexCount, uint16_t* indices, uint32_t indexCount)
{
    if (m_pVertexBuffer) m_pVertexBuffer->Release();
    m_pVertexBuffer = nullptr;
    if (m_pIndexBuffer) m_pIndexBuffer->Release();
    m_pIndexBuffer = nullptr;
    m_count = 0;
    if (!vertexCount || !vertices || !indices || !indexCount) return false;

    auto pRenderer = dynamic_cast<Renderer_d3d11*>(Game::getGame()->getComponent<Renderer>());
    if (!pRenderer) return false;
    auto pDevice = pRenderer->getDevice();
    if (!pDevice) return false;

    // Create the vertex buffer
    // Set up the description of the static vertex buffer.
    D3D11_BUFFER_DESC vertexBufferDesc;
    vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    vertexBufferDesc.ByteWidth = sizeof(sVertex) * vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    // Give the subresource structure a pointer to the vertex data.
    D3D11_SUBRESOURCE_DATA vertexData;
    vertexData.pSysMem = vertices;
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    if (pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertexBuffer) != S_OK)
    {
        m_pVertexBuffer = nullptr;
        return false;
    }

    // Create the index buffer
    // Set up the description of the static index buffer.
    D3D11_BUFFER_DESC indexBufferDesc;
    indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    indexBufferDesc.ByteWidth = sizeof(uint16_t) * indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;

    // Give the subresource structure a pointer to the index data.
    D3D11_SUBRESOURCE_DATA indexData;
    indexData.pSysMem = indices;
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    if (pDevice->CreateBuffer(&indexBufferDesc, &indexData, &m_pIndexBuffer) != S_OK)
    {
        m_pVertexBuffer->Release();
        m_pVertexBuffer = nullptr;
        m_pIndexBuffer = nullptr;
        return false;
    }

    m_count = indexCount;

    return true;
}

#endif
