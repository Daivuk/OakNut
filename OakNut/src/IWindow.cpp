#include "IWindow.h"

onut::IWindow::IWindow()
{
}

onut::IWindow::~IWindow()
{
}

const glm::tvec2<int>& onut::IWindow::getRealResolution() const
{
    return m_realResolution;
}
