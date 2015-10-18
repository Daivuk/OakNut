#if defined(ONUT_RENDERER_D3D11)
#include "Camera.h"
#include "ComponentManager.h"
#include "DirectionalLight.h"
#include "Entity.h"
#include "InputLayoutFactory_d3d11.h"
#include "Material.h"
#include "Mesh_d3d11.h"
#include "PointLight.h"
#include "Renderer_d3d11.h"
#include "SceneManager.h"
#include "Texture_d3d11.h"
#include "Window_win.h"

#include <fstream>
#include "glm/gtc/type_ptr.hpp"
#include <vector>

onut::Renderer_d3d11::~Renderer_d3d11()
{
    if (m_pCurrentMesh) m_pCurrentMesh->release();
    if (m_pCurrentInputLayout) m_pCurrentInputLayout->Release();
    if (m_pInputLayoutFactory) delete m_pInputLayoutFactory;
    if (m_pCurrentVertexShader) m_pCurrentVertexShader->Release();
    if (m_pCurrentPixelShader) m_pCurrentPixelShader->Release();

    if (m_pWhiteTexture) m_pWhiteTexture->release();
    if (m_pFlatNormalTexture) m_pFlatNormalTexture->release();
    if (m_pBlackTexture) m_pBlackTexture->release();

    for (auto& kv : m_shaders)
    {
        if (kv.second.first) kv.second.first->Release();
        if (kv.second.second) kv.second.second->Release();
    }

    if (m_pForwardDepthState) m_pForwardDepthState->Release();
    if (m_pForwardRasterizerState) m_pForwardRasterizerState->Release();
    if (m_pForwardBlendState) m_pForwardBlendState->Release();
    if (m_pForwardSamplerState) m_pForwardSamplerState->Release();

    if (m_pViewProjMatrixBuffer) m_pViewProjMatrixBuffer->Release();
    if (m_pModelMatrixBuffer) m_pModelMatrixBuffer->Release();
    if (m_pMaterialBuffer) m_pMaterialBuffer->Release();
    if (m_pViewBuffer) m_pViewBuffer->Release();
    if (m_pLightsBuffer) m_pLightsBuffer->Release();

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

    m_pInputLayoutFactory = new InputLayoutFactory_d3d11(this);

    // Create extra resources
    m_pWhiteTexture = Texture::create();
    m_pWhiteTexture->retain();
    uint32_t whitePixel = 0xffffffff;
    m_pWhiteTexture->setData(1, 1, &whitePixel);

    m_pFlatNormalTexture = Texture::create();
    m_pFlatNormalTexture->retain();
    uint32_t flatNormalPixel = 0xffff8080;
    m_pFlatNormalTexture->setData(1, 1, &flatNormalPixel);

    m_pBlackTexture = Texture::create();
    m_pBlackTexture->retain();
    uint32_t blackPixel = 0xff000000;
    m_pBlackTexture->setData(1, 1, &blackPixel);
}

void onut::Renderer_d3d11::end()
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
    swapChainDesc.SampleDesc.Quality = 0;
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

void onut::Renderer_d3d11::createProgram(const std::string& name, const std::vector<Mesh::eElement>& layout)
{
    //TODO:
#if defined(_DEBUG)
    std::ifstream vsFile("Debug/v" + name + ".cso", std::ios::binary);
    std::ifstream psFile("Debug/p" + name + ".cso", std::ios::binary);
#else
    std::ifstream vsFile("Release/v" + name + ".cso", std::ios::binary);
    std::ifstream psFile("Release/" + name + ".cso", std::ios::binary);
#endif

    std::vector<char> vsData = {std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>()};
    std::vector<char> psData = {std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>()};

    ID3D11VertexShader* pVertexShader = nullptr;
    ID3D11PixelShader* pPixelShader = nullptr;

    auto ret = m_pDevice->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &pVertexShader);
    assert(ret == S_OK);
    ret = m_pDevice->CreatePixelShader(psData.data(), psData.size(), nullptr, &pPixelShader);
    assert(ret == S_OK);

    m_vsShaderBlobs.push_back(std::make_pair(layout, std::move(vsData)));
    m_shaders.push_back(std::make_pair(layout, std::make_pair(pVertexShader, pPixelShader)));
}

void onut::Renderer_d3d11::createShaders()
{
    createProgram("Forward", {
        Mesh::eElement::POSITION, 
        Mesh::eElement::NORMAL, 
        Mesh::eElement::TEXCOORD});
    createProgram("ForwardColor", {
        Mesh::eElement::POSITION, 
        Mesh::eElement::NORMAL, 
        Mesh::eElement::TEXCOORD, 
        Mesh::eElement::COLOR});
    createProgram("ForwardBump", {
        Mesh::eElement::POSITION, 
        Mesh::eElement::NORMAL,
        Mesh::eElement::TANGENT,
        Mesh::eElement::BINORMAL,
        Mesh::eElement::TEXCOORD});
    createProgram("ForwardBumpColor", {
        Mesh::eElement::POSITION, 
        Mesh::eElement::NORMAL, 
        Mesh::eElement::TANGENT, 
        Mesh::eElement::BINORMAL,
        Mesh::eElement::TEXCOORD, 
        Mesh::eElement::COLOR});
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

    // View
    {
        D3D11_BUFFER_DESC cbDesc = CD3D11_BUFFER_DESC(sizeof(cbView), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
        auto ret = m_pDevice->CreateBuffer(&cbDesc, nullptr, &m_pViewBuffer);
        assert(ret == S_OK);
    }

    // Material
    {
        D3D11_BUFFER_DESC cbDesc = CD3D11_BUFFER_DESC(sizeof(cbMaterial), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
        auto ret = m_pDevice->CreateBuffer(&cbDesc, nullptr, &m_pMaterialBuffer);
        assert(ret == S_OK);
    }

    // Lights
    {
        D3D11_BUFFER_DESC cbDesc = CD3D11_BUFFER_DESC(sizeof(cbLights), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
        auto ret = m_pDevice->CreateBuffer(&cbDesc, nullptr, &m_pLightsBuffer);
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

    // View proj
    {
        D3D11_MAPPED_SUBRESOURCE map;
        m_pDeviceContext->Map(m_pViewProjMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
        *(glm::mat4*)map.pData = glm::transpose(viewProj);
        m_pDeviceContext->Unmap(m_pViewProjMatrixBuffer, 0);
        m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pViewProjMatrixBuffer);
    }

    // View
    {
        D3D11_MAPPED_SUBRESOURCE map;
        m_pDeviceContext->Map(m_pViewBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);

        auto &view = *(cbView*)map.pData;
        view.viewDir = pCamera->getViewDir();

        m_pDeviceContext->Unmap(m_pViewBuffer, 0);
        m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pViewBuffer);
    }
}

ID3D11Device* onut::Renderer_d3d11::getDevice() const
{
    return m_pDevice;
}

ID3D11DeviceContext* onut::Renderer_d3d11::getDeviceContext() const
{
    return m_pDeviceContext;
}

onut::InputLayoutFactory_d3d11* onut::Renderer_d3d11::getInputLayoutFactory() const
{
    return m_pInputLayoutFactory;
}

const std::vector<char>& onut::Renderer_d3d11::getVertexShaderBlobForLayout(const std::vector<Mesh::eElement>& layout) const
{
    for (auto& kv : m_vsShaderBlobs)
    {
        if (kv.first == layout) return kv.second;
    }
    assert(false); // Unsupported vertex layout by OakNut. Custom vertex layout unsupported in version 1.0
    return m_vsShaderBlobs[0].second;
}

void onut::Renderer_d3d11::getProgramForLayout(const std::vector<Mesh::eElement>& layout, ID3D11VertexShader** ppVertexShader, ID3D11PixelShader** ppPixelShader) const
{
    for (auto& kv : m_shaders)
    {
        if (kv.first == layout)
        {
            *ppVertexShader = kv.second.first;
            *ppPixelShader = kv.second.second;
            return;
        }
    }
    assert(false); // Unsupported vertex layout by OakNut. Custom vertex layout unsupported in version 1.0
    *ppVertexShader = nullptr;
    *ppPixelShader = nullptr;
}

void onut::Renderer_d3d11::begin()
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

void onut::Renderer_d3d11::draw(Mesh* pMesh,
                                Material* pMaterial,
                                const glm::mat4& transform,
                                const glm::vec4& ambientColor,
                                const std::vector<onut::PointLight*>& pointLights,
                                const std::vector<onut::DirectionalLight*>& directionalLights)
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
    auto pInputLayout = pMesh_d3d11->getInputLayout();
    if (!pInputLayout) return;
    auto pVertexShader = pMesh_d3d11->getVertexShader();
    if (!pVertexShader) return;
    auto pPixelShader = pMesh_d3d11->getPixelShader();
    if (!pPixelShader) return;

    // Set up lights
    {
        D3D11_MAPPED_SUBRESOURCE map;
        m_pDeviceContext->Map(m_pLightsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);

        auto &lights = *(cbLights*)map.pData;

        // Ambient
        lights.ambientColor = ambientColor;

        // Point lights
        lights.pointLightCount = static_cast<int32_t>(pointLights.size());
        for (int32_t i = 0; i < lights.pointLightCount && i < MAX_POINT_LIGHTS; ++i)
        {
            auto pLightEntity = dynamic_cast<Entity*>(pointLights[i]->getComponentManager());
            lights.pointLights[i].position = glm::vec3{pLightEntity->getWorldMatrix()[3]};
            lights.pointLights[i].radius = pointLights[i]->getRadius();
            lights.pointLights[i].color = pointLights[i]->getColor();
        }

        // Directionals
        lights.directionalLightCount = static_cast<int32_t>(directionalLights.size());
        for (int32_t i = 0; i < lights.directionalLightCount && i < MAX_DIRECTIONAL_LIGHTS; ++i)
        {
            auto pLightEntity = dynamic_cast<Entity*>(directionalLights[i]->getComponentManager());
            lights.directionalLights[i].dir = -glm::vec3(pLightEntity->getWorldMatrix()[1]);
            lights.directionalLights[i].color = directionalLights[i]->getColor();
        }

        m_pDeviceContext->Unmap(m_pLightsBuffer, 0);
        m_pDeviceContext->PSSetConstantBuffers(2, 1, &m_pLightsBuffer);
    }

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
        m_pDeviceContext->PSSetConstantBuffers(1, 1, &m_pMaterialBuffer);
    }

    // Bind textures
    ID3D11ShaderResourceView *pResourceViews[3];
    {
        auto pDiffuse = pMaterial->getDiffuse();
        if (!pDiffuse) pDiffuse = m_pWhiteTexture;
        auto pDiffuseD3D11 = dynamic_cast<Texture_d3d11*>(pDiffuse);
        assert(pDiffuseD3D11);
        pResourceViews[0] = pDiffuseD3D11->getResourceView();
    }
    {
        auto pNormalMap = pMaterial->getNormalMap();
        if (!pNormalMap) pNormalMap = m_pFlatNormalTexture;
        auto pNormalMapD3D11 = dynamic_cast<Texture_d3d11*>(pNormalMap);
        assert(pNormalMapD3D11);
        pResourceViews[1] = pNormalMapD3D11->getResourceView();
    }
    {
        auto pMaterialMap = pMaterial->getMaterialMap();
        if (!pMaterialMap) pMaterialMap = m_pBlackTexture;
        auto pMaterialMapD3D11 = dynamic_cast<Texture_d3d11*>(pMaterialMap);
        assert(pMaterialMapD3D11);
        pResourceViews[2] = pMaterialMapD3D11->getResourceView();
    }
    m_pDeviceContext->PSSetShaderResources(0, 3, pResourceViews);

    // Bind program
    if (pMesh != m_pCurrentMesh)
    {
        if (m_pCurrentMesh) m_pCurrentMesh->release();
        m_pCurrentMesh = pMesh;
        m_pCurrentMesh->retain();

        if (m_pCurrentInputLayout != pInputLayout)
        {
            if (m_pCurrentInputLayout) m_pCurrentInputLayout->Release();
            m_pCurrentInputLayout = pInputLayout;
            m_pCurrentInputLayout->AddRef();
            m_pDeviceContext->IASetInputLayout(m_pCurrentInputLayout);
        }
        if (m_pCurrentVertexShader != pVertexShader)
        {
            if (m_pCurrentVertexShader) m_pCurrentVertexShader->Release();
            m_pCurrentVertexShader = pVertexShader;
            m_pCurrentVertexShader->AddRef();
            m_pDeviceContext->VSSetShader(m_pCurrentVertexShader, nullptr, 0);
        }
        if (m_pCurrentPixelShader != pPixelShader)
        {
            if (m_pCurrentPixelShader) m_pCurrentPixelShader->Release();
            m_pCurrentPixelShader = pPixelShader;
            m_pCurrentPixelShader->AddRef();
            m_pDeviceContext->PSSetShader(m_pCurrentPixelShader, nullptr, 0);
        }

        // Draw using the mesh buffers
        UINT stride = static_cast<UINT>(pMesh_d3d11->getStride());
        UINT offset = 0;
        m_pDeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
        m_pDeviceContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
    }
    m_pDeviceContext->DrawIndexed(count, 0, 0);
}

#endif
