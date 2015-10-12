#if defined(ONUT_RENDERER_D3D11)
#include "InputLayoutFactory_d3d11.h"
#include "Renderer_d3d11.h"

onut::InputLayoutFactory_d3d11::InputLayoutFactory_d3d11(Renderer_d3d11* pRenderer)
    : m_pRenderer(pRenderer)
{
    m_pRenderer->retain();
    m_pDevice = pRenderer->getDevice();
    m_pDeviceContext = pRenderer->getDeviceContext();

    m_pDevice->AddRef();
    m_pDeviceContext->AddRef();
}

onut::InputLayoutFactory_d3d11::~InputLayoutFactory_d3d11()
{
    for (auto& kv : m_inputLayouts)
    {
        kv.second->Release();
    }
    m_pDevice->Release();
    m_pDeviceContext->Release();
    m_pRenderer->release();
}

ID3D11InputLayout* onut::InputLayoutFactory_d3d11::createInputLayout(const std::vector<onut::Mesh::eElement>& elements)
{
    // Find it
    for (auto& kv : m_inputLayouts)
    {
        if (kv.first == elements)
        {
            return kv.second;
        }
    }

    // Create it
    std::vector<D3D11_INPUT_ELEMENT_DESC> d3dLayout;
    UINT customIndex = 0;

    for (auto& element : elements)
    {
        D3D11_INPUT_ELEMENT_DESC elementDesc;
        elementDesc.SemanticIndex = 0;
        elementDesc.AlignedByteOffset = 0;
        elementDesc.InputSlot = 0;
        elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        elementDesc.InstanceDataStepRate = 0;
        switch (element)
        {
            case Mesh::eElement::POSITION:
                elementDesc.SemanticName = "POSITION";
                elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
                break;
            case Mesh::eElement::NORMAL:
                elementDesc.SemanticName = "NORMAL";
                elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
                break;
            case Mesh::eElement::TANGENT:
                elementDesc.SemanticName = "TANGENT";
                elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
                break;
            case Mesh::eElement::BINORMAL:
                elementDesc.SemanticName = "BINORMAL";
                elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
                break;
            case Mesh::eElement::TEXCOORD:
                elementDesc.SemanticName = "TEXCOORD";
                elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
                break;
            case Mesh::eElement::COLOR:
                elementDesc.SemanticName = "COLOR";
                elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
                break;
            case Mesh::eElement::CUSTOM1:
                elementDesc.SemanticName = "CUSTOM";
                elementDesc.SemanticIndex = customIndex++;
                elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
                break;
            case Mesh::eElement::CUSTOM2:
                elementDesc.SemanticName = "CUSTOM";
                elementDesc.SemanticIndex = customIndex++;
                elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
                break;
            case Mesh::eElement::CUSTOM3:
                elementDesc.SemanticName = "CUSTOM";
                elementDesc.SemanticIndex = customIndex++;
                elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
                break;
            case Mesh::eElement::CUSTOM4:
                elementDesc.SemanticName = "CUSTOM";
                elementDesc.SemanticIndex = customIndex++;
                elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
                break;
        }
        if (!d3dLayout.empty()) elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
        d3dLayout.push_back(elementDesc);
    }

    auto& shaderBlob = m_pRenderer->getVertexShaderBlobForLayout(elements);

    ID3D11InputLayout* pInputLayout = nullptr;
    auto ret = m_pDevice->CreateInputLayout(d3dLayout.data(),
                                            static_cast<UINT>(elements.size()), 
                                            shaderBlob.data(), shaderBlob.size(),
                                            &pInputLayout);
    assert(ret == S_OK && pInputLayout);
    m_inputLayouts.push_back(std::make_pair(elements, pInputLayout));
    return pInputLayout;
}

#endif
