#if defined(ONUT_RENDERER_D3D11)
#include "ComponentManager.h"
#include "Renderer_d3d11.h"
#include "Window_win.h"

onut::Renderer_d3d11::~Renderer_d3d11()
{
    if (m_pRenderTargetView) m_pRenderTargetView->Release();
    if (m_pDeviceContext) m_pDeviceContext->Release();
    if (m_pDevice) m_pDevice->Release();
    if (m_pSwapChain) m_pSwapChain->Release();
}

void onut::Renderer_d3d11::onCreate()
{
    createDevice();
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
}

void onut::Renderer_d3d11::setCamera(Camera* pCamera)
{
}

void onut::Renderer_d3d11::draw(Mesh* pMesh, Material* pMaterial, const glm::mat4& transform)
{
}

#endif
