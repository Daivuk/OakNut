#if defined(WIN32)
#include <Windows.h>
#include "Main.h"

int CALLBACK WinMain(
    _In_  HINSTANCE hInstance,
    _In_  HINSTANCE hPrevInstance,
    _In_  LPSTR lpCmdLine,
    _In_  int nCmdShow)
{
    onut::Main::main();
}
#endif
