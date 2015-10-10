#pragma once
#include "Resource.h"

namespace onut
{
    class Mesh : public Resource
    {
    public:
        static Mesh* create();

        bool load(const std::string& filename);

    protected:
        Mesh() {}
        virtual ~Mesh() {}
    };
}
