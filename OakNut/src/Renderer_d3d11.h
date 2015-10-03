#pragma once
#if defined(ONUT_RENDERER_D3D11)
#include "IRenderer.h"

#include <d3d11.h>

namespace onut
{
    class Window_win;

    class Renderer_d3d11 : public IRenderer
    {
    public:
        Renderer_d3d11(IWindow* pWindow);
        virtual ~Renderer_d3d11();

        void onCreate() override;
        void onDraw() override;

    private:
        void createDevice();
        void createRenderTargets();

        // Device stuff
        IDXGISwapChain* m_pSwapChain = nullptr;
        ID3D11Device* m_pDevice = nullptr;
        ID3D11DeviceContext* m_pDeviceContext = nullptr;

        // Render target
        ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
        D3D11_TEXTURE2D_DESC m_backBufferDesc;

        Window_win* m_pWindow = nullptr;
    };
}

#endif
