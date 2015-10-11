#include "LodePNG.h"
#include "Texture.h"

#include <stdio.h>

const glm::ivec2& onut::Texture::getSize() const
{
    return m_size;
}

bool onut::Texture::load(const std::string& filename)
{
    std::vector<unsigned char> image;
    unsigned int w, h;
    auto ret = lodepng::decode(image, w, h, filename);
    if (ret) return false;

    return setData(w, h, reinterpret_cast<uint32_t*>(image.data()));
}
