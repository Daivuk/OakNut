#if defined(WIN32)
#include "Game.h"
#include "StringUtils.h"
#include "Window_win.h"

onut::IWindow* onut::IWindow::createWindow()
{
    return new Window_win();
}

LRESULT CALLBACK WinProc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (msg == WM_DESTROY ||
        msg == WM_CLOSE)
    {
        PostQuitMessage(0);
        return 0;
    }
    else if (msg == WM_SIZE)
    {
        RECT clientRect;
        GetClientRect(handle, &clientRect);
        onut::Game::getGame()->getComponent<onut::IWindow>()->setResolution({clientRect.right - clientRect.left, clientRect.bottom - clientRect.top});
        return 0;
    }
    else if (msg == WM_SETCURSOR)
    {
        //if (pWindow->m_cursor)
        //{
        //    SetCursor(pWindow->m_cursor);
        //    return 0;
        //}
    }
    else if (msg == WM_SYSCOMMAND)
    {
        if (wparam == SC_KEYMENU && (lparam >> 16) <= 0)
        {
            return 0;
        }
    }
    else if (msg == WM_CHAR)
    {
        auto c = (char)wparam;
        //if (OWindow->onWrite)
        //{
        //    OWindow->onWrite(c);
        //}
        return 0;
    }
    else if (msg == WM_KEYDOWN)
    {
        //if (OWindow->onKey)
        //{
        //    OWindow->onKey(static_cast<uintptr_t>(wparam));
        //}
        return 0;
    }
    else if (msg == WM_COMMAND)
    {
        //if (OWindow->onMenu)
        //{
        //    OWindow->onMenu(LOWORD(wparam));
        //}
        return 0;
    }

    return DefWindowProc(handle, msg, wparam, lparam);
}

void onut::Window_win::onCreate()
{
    auto isFullscreen = false;
    auto isResizable = false;

    // Define window style
    WNDCLASS wc = {0};
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = WinProc;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.lpszClassName = TEXT("OakNutWindow");
    RegisterClass(&wc);

    // Centered position
    auto screenW = GetSystemMetrics(SM_CXSCREEN);
    auto screenH = GetSystemMetrics(SM_CYSCREEN);

    auto windowNameUTF16 = onut::utf8ToUtf16(getCaption());
    auto resolution = getResolution();

    if (isFullscreen)
    {
        long posX = 0;
        long posY = 0;
        m_handle = CreateWindow(TEXT("OakNutWindow"),
                                windowNameUTF16.c_str(),
                                WS_POPUP | WS_VISIBLE,
                                posX, posY, screenW, screenH,
                                nullptr, nullptr, nullptr, nullptr);
    }
    else
    {
        auto posX = (screenW - resolution.x) / 2;
        auto posY = (screenH - resolution.y) / 2;

        // Create the window
        if (!isResizable)
        {
            m_handle = CreateWindow(TEXT("OakNutWindow"),
                                    windowNameUTF16.c_str(),
                                    WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
                                    posX, posY, resolution.x, resolution.y,
                                    nullptr, nullptr, nullptr, nullptr);
        }
        else
        {
            m_handle = CreateWindow(TEXT("OakNutWindow"),
                                    windowNameUTF16.c_str(),
                                    WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                    posX, posY, resolution.x, resolution.y,
                                    nullptr, nullptr, nullptr, nullptr);
        }
    }

    RECT clientRect;
    GetClientRect(m_handle, &clientRect);
    m_realResolution = {clientRect.right - clientRect.left, clientRect.bottom - clientRect.top};
}

void onut::Window_win::onUpdate()
{
    MSG msg = {0};
    if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if (msg.message == WM_QUIT)
        {
            setEnabled(false);
        }
    }
}

HWND onut::Window_win::getHandle() const
{
    return m_handle;
}

#endif
