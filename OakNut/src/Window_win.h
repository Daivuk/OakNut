#pragma once
#if defined(WIN32)
#include "Window.h"

#include <Windows.h>

namespace onut
{
    class Window_win : public Window
    {
    public:
        void onCreate() override;
        void onUpdate() override;

        HWND getHandle() const;

    private:
        HWND m_handle = nullptr;
    };
}

#endif
