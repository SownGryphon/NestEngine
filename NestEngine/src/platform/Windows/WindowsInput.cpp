#include "WindowsInput.h"

#include <GLFW/glfw3.h>

#include "Nest/Core/Application.h"

namespace Nest
{
    Scope<Input> Input::s_instance = createScope<WindowsInput>();

    bool Nest::WindowsInput::isKeyPressedImpl(int keyCode)
    {
        auto window = static_cast<GLFWwindow*>(Application::GetInstance().getWindow().getNativeWindow());
        auto state = glfwGetKey(window, keyCode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Nest::WindowsInput::isMouseButtonDownImpl(int button)
    {
        auto window = static_cast<GLFWwindow*>(Application::GetInstance().getWindow().getNativeWindow());
        auto buttonState = glfwGetMouseButton(window, button);
        return buttonState == GLFW_PRESS;
    }

    chcl::Vector2<float> Nest::WindowsInput::getMousePosImpl()
    {
        auto window = static_cast<GLFWwindow*>(Application::GetInstance().getWindow().getNativeWindow());
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        return chcl::Vector2<float>(xpos, ypos);
    }
}