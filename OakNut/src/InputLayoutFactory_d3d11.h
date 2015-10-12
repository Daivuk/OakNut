#pragma once
#if defined(ONUT_RENDERER_D3D11)
#include "Mesh.h"

#include <d3d11.h>
#include <unordered_map>

namespace onut
{
    class Renderer_d3d11;

    class InputLayoutFactory_d3d11
    {
    public:
        InputLayoutFactory_d3d11(Renderer_d3d11* pRenderer);
        virtual ~InputLayoutFactory_d3d11();

        ID3D11InputLayout* createInputLayout(const std::vector<onut::Mesh::eElement>& elements);

    private:
        ID3D11Device* m_pDevice = nullptr;
        ID3D11DeviceContext* m_pDeviceContext = nullptr;
        Renderer_d3d11* m_pRenderer = nullptr;

        std::vector<std::pair<std::vector<onut::Mesh::eElement>, ID3D11InputLayout*>> m_inputLayouts;
    };
};

#endif
