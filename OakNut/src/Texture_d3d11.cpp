#if defined(ONUT_RENDERER_D3D11)
#include "Game.h"
#include "Renderer_d3d11.h"
#include "Texture_d3d11.h"

onut::Texture* onut::Texture::create()
{
    return new Texture_d3d11();
}

onut::Texture_d3d11::~Texture_d3d11()
{
    if (m_pResourceView) m_pResourceView->Release();
    if (m_pTexture) m_pTexture->Release();
}

ID3D11ShaderResourceView* onut::Texture_d3d11::getResourceView() const
{
    return m_pResourceView;
}

bool onut::Texture_d3d11::setData(uint32_t width, uint32_t height, uint32_t* pData)
{
    m_size.x = 0;
    m_size.y = 0;
    if (m_pResourceView) m_pResourceView->Release();
    m_pResourceView = nullptr;
    if (m_pTexture) m_pTexture->Release();
    m_pTexture = nullptr;

    auto pRenderer = dynamic_cast<Renderer_d3d11*>(Game::getGame()->getComponent<Renderer>());
    if (!pRenderer) return false;
    auto pDevice = pRenderer->getDevice();
    if (!pDevice) return false;

    if (!pData) return false;
    if (!width) return false;
    if (!height) return false;

    m_size.x = static_cast<int>(width);
    m_size.y = static_cast<int>(height);

    // Manually generate mip levels
    bool allowMipMaps = true;
    UINT w2 = 1;
    UINT h2 = 1;
    while (w2 < (UINT)m_size.x) w2 *= 2;
    if (m_size.x != w2) allowMipMaps = false;
    while (h2 < (UINT)m_size.y) h2 *= 2;
    if (m_size.y != h2) allowMipMaps = false;
    unsigned char* pMipMaps = NULL;
    int mipLevels = 1;
    D3D11_SUBRESOURCE_DATA* mipsData = NULL;
    //allowMipMaps = allowMipMaps && in_generateMipmaps;
    if (allowMipMaps)
    {
        UINT biggest = max(w2, h2);
        UINT w2t = w2;
        UINT h2t = h2;
        UINT totalSize = w2t * h2t * 4;
        while (!(w2t == 1 && h2t == 1))
        {
            ++mipLevels;
            w2t /= 2;
            if (w2t < 1) w2t = 1;
            h2t /= 2;
            if (h2t < 1) h2t = 1;
            totalSize += w2t * h2t * 4;
        }
        pMipMaps = new byte[totalSize];
        memcpy(pMipMaps, pData, m_size.x * m_size.y * 4);

        mipsData = new D3D11_SUBRESOURCE_DATA[mipLevels];

        w2t = w2;
        h2t = h2;
        totalSize = 0;
        int mipTarget = mipLevels;
        mipLevels = 0;
        byte* prev;
        byte* cur;
        while (mipLevels != mipTarget)
        {
            prev = pMipMaps + totalSize;
            mipsData[mipLevels].pSysMem = prev;
            mipsData[mipLevels].SysMemPitch = w2t * 4;
            mipsData[mipLevels].SysMemSlicePitch = 0;
            totalSize += w2t * h2t * 4;
            cur = pMipMaps + totalSize;
            w2t /= 2;
            if (w2t < 1) w2t = 1;
            h2t /= 2;
            if (h2t < 1) h2t = 1;
            ++mipLevels;
            if (mipLevels == mipTarget) break;
            int accum;

            // Generate the mips
            int multX = w2 / w2t;
            int multY = h2 / h2t;
            for (UINT y = 0; y < h2t; ++y)
            {
                for (UINT x = 0; x < w2t; ++x)
                {
                    for (UINT k = 0; k < 4; ++k)
                    {
                        accum = 0;
                        accum += prev[(y * multY * w2 + x * multX) * 4 + k];
                        accum += prev[(y * multY * w2 + (x + multX / 2) * multX) * 4 + k];
                        accum += prev[((y + multY / 2) * multY * w2 + x * multX) * 4 + k];
                        accum += prev[((y + multY / 2) * multY * w2 + (x + multX / 2) * multX) * 4 + k];
                        cur[(y * w2t + x) * 4 + k] = accum / 4;
                    }
                }
            }

            w2 = w2t;
            h2 = h2t;
        }
    }

    D3D11_TEXTURE2D_DESC desc;
    desc.Width = m_size.x;
    desc.Height = m_size.y;
    desc.MipLevels = mipLevels;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_IMMUTABLE;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA data;
    data.pSysMem = (pMipMaps) ? reinterpret_cast<void*>(pMipMaps) : reinterpret_cast<void*>(pData);
    data.SysMemPitch = m_size.x * 4;
    data.SysMemSlicePitch = 0;

    auto ret = pDevice->CreateTexture2D(&desc, (mipsData) ? mipsData : &data, &m_pTexture);
    if (ret != S_OK)
    {
        if (pMipMaps) delete[] pMipMaps;
        if (mipsData) delete[] mipsData;
        return false;
    }
    ret = pDevice->CreateShaderResourceView(m_pTexture, NULL, &m_pResourceView);
    if (ret != S_OK)
    {
        m_pTexture->Release();
        m_pTexture = nullptr;
        if (pMipMaps) delete[] pMipMaps;
        if (mipsData) delete[] mipsData;
        return false;
    }

    if (pMipMaps) delete[] pMipMaps;
    if (mipsData) delete[] mipsData;

    return true;
}

#endif
