#pragma once
#if defined(ONUT_RENDERER_D3D11)
#include "Renderer.h"

#include <d3d11.h>

namespace onut
{
    class Window_win;
    class Texture;

    class Renderer_d3d11 : public Renderer
    {
    public:
        virtual ~Renderer_d3d11();

        void onUpdate(const onut::TimeInfo& timeInfo) override;
        void onCreate() override;
        void onDraw() override;

        void setCamera(Camera* pCamera) override;
        void draw(Mesh* pMesh, Material* pMaterial, const glm::mat4& transform) override;

        ID3D11Device* getDevice() const;
        ID3D11DeviceContext* getDeviceContext() const;

    private:
        void createDevice();
        void createRenderTargets();
        void createShaders();
        void createConstantBuffers();
        void createStates();

        struct cbMaterial
        {
            glm::vec4 tint;
            float reflectivity;
            float metallic;
            float roughness;
            float cbMaterial_padding;
        };

        struct cbView
        {
            glm::vec3 viewDir;
            float cbView_padding;
        };

        // Device stuff
        IDXGISwapChain* m_pSwapChain = nullptr;
        ID3D11Device* m_pDevice = nullptr;
        ID3D11DeviceContext* m_pDeviceContext = nullptr;

        // Render target
        ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
        ID3D11DepthStencilView* m_pDepthStencilView = nullptr;
        D3D11_TEXTURE2D_DESC m_backBufferDesc;

        // Shaders
        ID3D11VertexShader* m_pForwardVertexShader = nullptr;
        ID3D11PixelShader* m_pForwardPixelShader = nullptr;
        ID3D11InputLayout* m_pForwardInputLayout = nullptr;

        // Constant buffers
        ID3D11Buffer* m_pViewProjMatrixBuffer = nullptr;
        ID3D11Buffer* m_pModelMatrixBuffer = nullptr;
        ID3D11Buffer* m_pViewBuffer = nullptr;
        ID3D11Buffer* m_pMaterialBuffer = nullptr;

        // Render states
        ID3D11DepthStencilState* m_pForwardDepthState = nullptr;
        ID3D11RasterizerState* m_pForwardRasterizerState = nullptr;
        ID3D11BlendState* m_pForwardBlendState = nullptr;
        ID3D11SamplerState* m_pForwardSamplerState = nullptr;

        // White texture for rendering without diffuse
        Texture* pWhiteTexture = nullptr;
        Texture* pBlackTexture = nullptr;
        Texture* pFlatNormalTexture = nullptr;
    };
}

#endif
