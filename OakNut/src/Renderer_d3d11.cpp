#if defined(ONUT_RENDERER_D3D11)
#include "Renderer_d3d11.h"
#include "Window_win.h"

onut::IRenderer* onut::IRenderer::createRenderer(IWindow* pWindow)
{
    return new Renderer_d3d11(pWindow);
}

onut::Renderer_d3d11::Renderer_d3d11(IWindow* pWindow)
{
    m_pWindow = dynamic_cast<Window_win*>(pWindow);
    m_pWindow->retain();
}

onut::Renderer_d3d11::~Renderer_d3d11()
{
    m_pWindow->release();

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
    // Define our swap chain
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {0};
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = m_pWindow->getHandle();
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

#endif
