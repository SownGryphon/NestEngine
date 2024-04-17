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
}