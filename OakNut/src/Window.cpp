#include "Window.h"

onut::Window::Window()
{
}

onut::Window::~Window()
{
}

const glm::tvec2<int>& onut::Window::getRealResolution() const
{
    return m_realResolution;
}
