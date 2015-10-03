#pragma once
namespace onut
{
    class Object
    {
    public:
        virtual ~Object();

        void retain();
        void release();
        int getRefCount() const;

    private:
        int m_refCount = 0;
    };
}
