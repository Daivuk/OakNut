#pragma once
#include "Resource.h"

namespace onut
{
    class Texture
        : public Resource
    {
    public:
        static Texture* create();

        const glm::ivec2& getSize() const;

        bool load(const std::string& filename);
        virtual bool setData(uint32_t width, uint32_t height, uint32_t* pData) = 0;

    protected:
        Texture() {}
        virtual ~Texture() {}

        glm::ivec2 m_size;
    };
}
