#if defined(WIN32)
#include "IWindow.h"

#include <Windows.h>

namespace onut
{
    class Window_win : public IWindow
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
