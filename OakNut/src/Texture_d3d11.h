#if defined(ONUT_RENDERER_D3D11)
#pragma once
#include "Texture.h"

#include <d3d11.h>

namespace onut
{
    class Texture_d3d11 : public Texture
    {
    public:
        ID3D11ShaderResourceView* getResourceView() const;
        bool setData(uint32_t width, uint32_t height, uint32_t* pData) override;

    protected:
        friend Texture;
        Texture_d3d11() {}
        virtual ~Texture_d3d11();

    private:
        ID3D11Texture2D* m_pTexture = nullptr;
        ID3D11ShaderResourceView* m_pResourceView = nullptr;
    };
};

#endif
