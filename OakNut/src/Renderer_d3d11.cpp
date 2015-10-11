#if defined(ONUT_RENDERER_D3D11)
#include "Camera.h"
#include "ComponentManager.h"
#include "Material.h"
#include "Mesh_d3d11.h"
#include "Renderer_d3d11.h"
#include "SceneManager.h"
#include "Window_win.h"

#include <fstream>
#include "glm/gtc/type_ptr.hpp"
#include <vector>

onut::Renderer_d3d11::~Renderer_d3d11()
{
    if (m_pForwardDepthState) m_pForwardDepthState->Release();
    if (m_pForwardRasterizerState) m_pForwardRasterizerState->Release();
    if (m_pForwardBlendState) m_pForwardBlendState->Release();
    if (m_pForwardSamplerState) m_pForwardSamplerState->Release();

    if (m_pViewProjMatrixBuffer) m_pViewProjMatrixBuffer->Release();
    if (m_pModelMatrixBuffer) m_pModelMatrixBuffer->Release();
    if (m_pMaterialBuffer) m_pMaterialBuffer->Release();

    if (m_pForwardVertexShader) m_pForwardVertexShader->Release();
    if (m_pForwardPixelShader) m_pForwardPixelShader->Release();
    if (m_pForwardInputLayout) m_pForwardInputLayout->Release();

    if (m_pDepthStencilView) m_pDepthStencilView->Release();
    if (m_pRenderTargetView) m_pRenderTargetView->Release();
    if (m_pDeviceContext) m_pDeviceContext->Release();
    if (m_pDevice) m_pDevice->Release();
    if (m_pSwapChain) m_pSwapChain->Release();
}

void onut::Renderer_d3d11::onCreate()
{
    createDevice();
    createRenderTargets();
    createShaders();
    createConstantBuffers();
    createStates();
}

void onut::Renderer_d3d11::onDraw()
{
    m_pSwapChain->Present(1, 0);
}

void onut::Renderer_d3d11::createDevice()
{
    auto pComponentManager = getComponentManager();
    if (!pComponentManager) return;
    auto pWindow = pComponentManager->getComponent<Window_win>();
    if (!pWindow) return;

    // Define our swap chain
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {0};
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = pWindow->getHandle();
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.Windowed = TRUE;

    // Create the swap chain, device and device context
    D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
#if _DEBUG
        D3D11_CREATE_DEVICE_DEBUG,
#else
        0,
#endif
        nullptr, 0, D3D11_SDK_VERSION,
        &swapChainDesc, &m_pSwapChain,
        &m_pDevice, nullptr, &m_pDeviceContext);
}

void onut::Renderer_d3d11::createRenderTargets()
{
    ID3D11Texture2D* pBackBuffer;
    m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
    m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
    pBackBuffer->GetDesc(&m_backBufferDesc);
    pBackBuffer->Release();

    // Initialize the description of the depth buffer.
    D3D11_TEXTURE2D_DESC depthBufferDesc;
    ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

    // Set up the description of the depth buffer.
    depthBufferDesc.Width = m_backBufferDesc.Width;
    depthBufferDesc.Height = m_backBufferDesc.Height;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDesc.CPUAccessFlags = 0;
    depthBufferDesc.MiscFlags = 0;

    // Create the texture for the depth buffer using the filled out description.
    ID3D11Texture2D* pDepthStencilTexture = nullptr;
    auto ret = m_pDevice->CreateTexture2D(&depthBufferDesc, NULL, &pDepthStencilTexture);
    assert(ret == S_OK);

    // Initailze the depth stencil view.
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

    // Set up the depth stencil view description.
    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    // Create the depth stencil view.
    ret = m_pDevice->CreateDepthStencilView(pDepthStencilTexture, &depthStencilViewDesc, &m_pDepthStencilView);
    assert(ret == S_OK);
    pDepthStencilTexture->Release();
}

void onut::Renderer_d3d11::createShaders()
{
#if defined(_DEBUG)
    std::ifstream vsFile("Debug/vForward.cso", std::ios::binary);
    std::ifstream psFile("Debug/pForward.cso", std::ios::binary);
#else
    std::ifstream vsFile("Release/vForward.cso", std::ios::binary);
    std::ifstream psFile("Release/pForward.cso", std::ios::binary);
#endif

    std::vector<char> vsData = {std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>()};
    std::vector<char> psData = {std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>()};

    auto ret = m_pDevice->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &m_pForwardVertexShader);
    assert(ret == S_OK);
    ret = m_pDevice->CreatePixelShader(psData.data(), psData.size(), nullptr, &m_pForwardPixelShader);
    assert(ret == S_OK);

    // Create input layout
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        {"V_ELEMENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"V_ELEMENT", 1, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"V_ELEMENT", 2, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"V_ELEMENT", 3, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    ret = m_pDevice->CreateInputLayout(layout, 4, vsData.data(), vsData.size(), &m_pForwardInputLayout);
    assert(ret == S_OK);

    // Bind the shaders
    m_pDeviceContext->VSSetShader(m_pForwardVertexShader, nullptr, 0);
    m_pDeviceContext->PSSetShader(m_pForwardPixelShader, nullptr, 0);
    m_pDeviceContext->IASetInputLayout(m_pForwardInputLayout);
}

void onut::Renderer_d3d11::createConstantBuffers()
{
    // View projection matrix
    {
        D3D11_BUFFER_DESC cbDesc = CD3D11_BUFFER_DESC(sizeof(float) * 16, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
        auto ret = m_pDevice->CreateBuffer(&cbDesc, nullptr, &m_pViewProjMatrixBuffer);
        assert(ret == S_OK);
    }

    // Model matrix
    {
        D3D11_BUFFER_DESC cbDesc = CD3D11_BUFFER_DESC(sizeof(float) * 16, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
        auto ret = m_pDevice->CreateBuffer(&cbDesc, nullptr, &m_pModelMatrixBuffer);
        assert(ret == S_OK);
    }

    // Material
    {
        D3D11_BUFFER_DESC cbDesc = CD3D11_BUFFER_DESC(sizeof(cbMaterial), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
        auto ret = m_pDevice->CreateBuffer(&cbDesc, nullptr, &m_pMaterialBuffer);
        assert(ret == S_OK);
    }
}

void onut::Renderer_d3d11::createStates()
{
    // I'm tired of rewritting that shit every engine. How could I generalize this?
    {
        D3D11_DEPTH_STENCIL_DESC desc;
        desc.DepthEnable = true;
        desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        desc.DepthFunc = D3D11_COMPARISON_LESS;
        desc.StencilEnable = false;
        desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
        desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
        desc.FrontFace = {D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS};
        desc.BackFace = {D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS};
        auto ret = m_pDevice->CreateDepthStencilState(&desc, &m_pForwardDepthState);
        assert(ret == S_OK);
    }
    {
        D3D11_RASTERIZER_DESC desc;
        desc.FillMode = D3D11_FILL_SOLID;
        desc.CullMode = D3D11_CULL_BACK;
        desc.FrontCounterClockwise = true;
        desc.DepthBias = 0;
        desc.DepthBiasClamp = 0.f;
        desc.SlopeScaledDepthBias = 0.f;
        desc.DepthClipEnable = true;
        desc.ScissorEnable = false;
        desc.MultisampleEnable = true;
        desc.AntialiasedLineEnable = false;
        auto ret = m_pDevice->CreateRasterizerState(&desc, &m_pForwardRasterizerState);
        assert(ret == S_OK);
    }
    {
        D3D11_BLEND_DESC desc;
        desc.AlphaToCoverageEnable = FALSE;
        desc.IndependentBlendEnable = FALSE;
        for (auto &rt : desc.RenderTarget)
        {
            rt = {
                FALSE,
                D3D11_BLEND_ONE,
                D3D11_BLEND_INV_SRC_ALPHA,
                D3D11_BLEND_OP_ADD,
                D3D11_BLEND_ONE,
                D3D11_BLEND_ONE,
                D3D11_BLEND_OP_ADD,
                D3D10_COLOR_WRITE_ENABLE_ALL
            };
        }
        auto ret = m_pDevice->CreateBlendState(&desc, &m_pForwardBlendState);
        assert(ret == S_OK);
    }
    {
        D3D11_SAMPLER_DESC desc;
        desc.Filter = D3D11_FILTER_ANISOTROPIC;
        desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.MipLODBias = 0.f;
        desc.MaxAnisotropy = 4;
        desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
        desc.BorderColor[0] = 0.f;
        desc.BorderColor[1] = 0.f;
        desc.BorderColor[2] = 0.f;
        desc.BorderColor[3] = 0.f;
        desc.MinLOD = 0;
        desc.MaxLOD = D3D11_FLOAT32_MAX;
        auto ret = m_pDevice->CreateSamplerState(&desc, &m_pForwardSamplerState);
        assert(ret == S_OK);
    }
}

void onut::Renderer_d3d11::setCamera(Camera* pCamera)
{
    if (!pCamera) return;
    auto viewProj = pCamera->getViewProj();

    // Set the constant buffer
    D3D11_MAPPED_SUBRESOURCE map;
    m_pDeviceContext->Map(m_pViewProjMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
    *(glm::mat4*)map.pData = glm::transpose(viewProj);
    m_pDeviceContext->Unmap(m_pViewProjMatrixBuffer, 0);
    m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pViewProjMatrixBuffer);
}

void onut::Renderer_d3d11::draw(Mesh* pMesh, Material* pMaterial, const glm::mat4& transform)
{
    // Some validation
    if (!pMaterial) return;
    auto pMesh_d3d11 = dynamic_cast<Mesh_d3d11*>(pMesh);
    if (!pMesh_d3d11) return;
    auto pVertexBuffer = pMesh_d3d11->getVertexBuffer();
    if (!pVertexBuffer) return;
    auto pIndexBuffer = pMesh_d3d11->getIndexBuffer();
    if (!pIndexBuffer) return;
    auto count = pMesh_d3d11->getCount();
    if (!count) return;

    // Set it's matrix
    {
        D3D11_MAPPED_SUBRESOURCE map;
        m_pDeviceContext->Map(m_pModelMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
        *(glm::mat4*)map.pData = glm::transpose(transform);
        m_pDeviceContext->Unmap(m_pModelMatrixBuffer, 0);
        m_pDeviceContext->VSSetConstantBuffers(1, 1, &m_pModelMatrixBuffer);
    }

    // Setup it's material
    {
        D3D11_MAPPED_SUBRESOURCE map;
        m_pDeviceContext->Map(m_pMaterialBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
        auto &material = *(cbMaterial*)map.pData;
        material.tint = pMaterial->getTint();
        material.reflectivity = pMaterial->getReflectivity();
        material.metallic = pMaterial->getMetallic();
        material.roughness = pMaterial->getRoughness();
        m_pDeviceContext->Unmap(m_pMaterialBuffer, 0);
        m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pMaterialBuffer);
    }

    // Draw using the mesh buffers
    UINT stride = sizeof(Mesh::sVertex);
    UINT offset = 0;
    m_pDeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
    m_pDeviceContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
    m_pDeviceContext->DrawIndexed(count, 0, 0);
}

ID3D11Device* onut::Renderer_d3d11::getDevice() const
{
    return m_pDevice;
}

ID3D11DeviceContext* onut::Renderer_d3d11::getDeviceContext() const
{
    return m_pDeviceContext;
}

void onut::Renderer_d3d11::onUpdate(const onut::TimeInfo& timeInfo)
{
    // Bind the render target view and depth stencil buffer to the output render pipeline.
    m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

    // Clear render targets
    FLOAT clearColor[] = {0.f, 0.f, 0.f, 1.f};
    m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, clearColor);
    m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    // Setup the viewport for rendering.
    D3D11_VIEWPORT viewport;
    viewport.Width = static_cast<float>(m_backBufferDesc.Width);
    viewport.Height = static_cast<float>(m_backBufferDesc.Height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    m_pDeviceContext->RSSetViewports(1, &viewport);

    // Set topology
    m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Set forward rendering states
    m_pDeviceContext->OMSetDepthStencilState(m_pForwardDepthState, 1);
    m_pDeviceContext->RSSetState(m_pForwardRasterizerState);
    m_pDeviceContext->OMSetBlendState(m_pForwardBlendState, nullptr, 0xffffffff);
    m_pDeviceContext->PSSetSamplers(0, 1, &m_pForwardSamplerState);
}

#endif
