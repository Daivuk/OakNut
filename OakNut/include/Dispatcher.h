#pragma once
#include "Component.h"
#include "Synchronous.h"

namespace onut
{
    class Dispatcher : public Component
    {
    public:
        void onUpdate(const onut::TimeInfo& timeInfo) override;
        
        template<typename Tfn, typename ... Targs>
        void dispatch(Tfn callback, Targs... args)
        {
            m_synchronous.sync(callback, args...);
        }
            
    private:
        onut::Synchronous<> m_synchronous;
    };
};
