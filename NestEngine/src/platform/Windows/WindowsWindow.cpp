#include "WindowsWindow.h"

#include <chrono>
#include <codecvt>
#include <iostream>
#include <thread>

#include <glad/gl.h>
#include <glad/wgl.h>

#include "platform/Windows/WindowsUtils.h"
#include "platform/Windows/PlatformWindows.h"

#include "Nest/Events/WindowEvent.h"
#include "Nest/Events/MouseEvent.h"
#include "Nest/Events/KeyboardEvent.h"

namespace Nest
{
    static bool s_GLFWInitialized = false;

    WindowsWindow::WindowsWindow(const WindowProps &props)
    {
        init(props);
    }

    WindowsWindow::~WindowsWindow()
    {
        shutdown();
        Window::~Window();
    }

    void Nest::WindowsWindow::processEvents()
    {
        MSG windowMessage;
        while (PeekMessage(&windowMessage, m_windowHandle, NULL, NULL, PM_REMOVE))
        {
            if (windowMessage.message == WM_QUIT)
            {
                NE_TRACE("Encountered quit message");
                break;
            }

            TranslateMessage(&windowMessage);
            DispatchMessage(&windowMessage);
        }
    }

    void WindowsWindow::onUpdate()
    {
        auto timeNow = std::chrono::high_resolution_clock::now();
        std::chrono::nanoseconds frameTime = std::chrono::nanoseconds(int(1'000'000'000 / m_winData.fps));
        if (m_winData.fps != 0 && timeNow - m_lastFrameTime < frameTime)
        {
            m_lastFrameTime += frameTime;
            std::this_thread::sleep_until(m_lastFrameTime);
        }
        else
            m_lastFrameTime = timeNow;
        UpdateWindow(m_windowHandle);
        SwapBuffers(m_winData.deviceContext);
    }

    unsigned int WindowsWindow::getWidth() const
    {
        return m_winData.width;
    }

    unsigned int WindowsWindow::getHeight() const
    {
        return m_winData.height;
    }

    void WindowsWindow::setVSync(bool enabled)
    {
        m_winData.vSync = enabled;
    }

    bool WindowsWindow::isVSync() const
    {
        return m_winData.vSync;
    }

    void WindowsWindow::setFPS(float fps)
    {
        m_winData.fps = fps;
    }

    void WindowsWindow::init(const WindowProps &props)
    {
        m_winData.width = props.width;
        m_winData.height = props.height;
        m_winData.title = props.title;
        m_winData.resizable = props.resizable;

        s_windowClass.lpfnWndProc = WindowsWindow::WindowProc;
        s_windowClass.hInstance = PlatformWindows::s_instanceHandle;
        s_windowClass.lpszClassName = s_windowClassName.c_str();

        RegisterClass(&s_windowClass);

        std::wstring wideWindowName = WindowsUtils::toWstring(m_winData.title);
        m_windowHandle = CreateWindowEx(
            0,
            s_windowClassName.c_str(),
            wideWindowName.c_str(),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            m_winData.width,
            m_winData.height,
            NULL,
            NULL,
            PlatformWindows::s_instanceHandle,
            &m_winData
        );

        if (m_windowHandle == NULL)
        {
            NE_ERROR("Failed to create window");
            return;
        }

        ShowWindow(m_windowHandle, SW_SHOWDEFAULT);
        UpdateWindow(m_windowHandle);

        NE_INFO("Created window {0} of size [{1}, {2}].", props.title, props.width, props.height);

        m_lastFrameTime = std::chrono::high_resolution_clock::time_point();
    }

    void WindowsWindow::shutdown()
    {
        if (m_winData.deviceContext && m_winData.openglRenderContext)
        {
            wglMakeCurrent(m_winData.deviceContext, NULL);
            wglDeleteContext(m_winData.openglRenderContext);
        }
    }

    LRESULT CALLBACK WindowsWindow::WindowProc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg)
        {
            case WM_CREATE:
            {
                NE_TRACE("Received window create message [{}]", uMsg);

                // Set user data for window
                CREATESTRUCT *createStructPtr = (CREATESTRUCT*)lParam;
                WindowData *winDataPtr = (WindowData*)createStructPtr->lpCreateParams;
                SetWindowLongPtr(windowHandle, GWLP_USERDATA, (LONG_PTR)winDataPtr);

                // Create OpenGL Context
                PIXELFORMATDESCRIPTOR pixelFormatDesc =
                {
                    sizeof(PIXELFORMATDESCRIPTOR),
                    1,
                    PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,	// Flags
                    PFD_TYPE_RGBA,	// Framebuffer type, RGBA or palette
                    32,				// Colordepth
                    0, 0, 0, 0, 0, 0,
                    0,
                    0,
                    0,
                    0, 0, 0, 0,
                    24,	// number of bits for depth buffer
                    8,	// Number of bits for stencil buffer
                    0,	// Number of aux bits
                    PFD_MAIN_PLANE,
                    0,
                    0, 0, 0
                };

                HDC deviceContext = GetDC(windowHandle);

                int pixelFormat = ChoosePixelFormat(deviceContext, &pixelFormatDesc);
                SetPixelFormat(deviceContext, pixelFormat, &pixelFormatDesc);

                HGLRC openglRenderContext = wglCreateContext(deviceContext);
                wglMakeCurrent(deviceContext, openglRenderContext);

                winDataPtr->deviceContext = deviceContext;
                winDataPtr->openglRenderContext = openglRenderContext;

                // Load OpenGL and WGL
                int openGlVersion = gladLoaderLoadGL();
                NE_INFO("GLAD loaded OpenGL version {}.{}", GLAD_VERSION_MAJOR(openGlVersion), GLAD_VERSION_MINOR(openGlVersion));
                int wglVersion = gladLoaderLoadWGL(deviceContext);
                NE_INFO("Glad loaded WGL version {}.{}", GLAD_VERSION_MAJOR(wglVersion), GLAD_VERSION_MINOR(wglVersion));

                return 0;
            }	// WM_CREATE

            case WM_PAINT:
            {
                PAINTSTRUCT paintStruct;
                HDC hdc = BeginPaint(windowHandle, &paintStruct);

                //FillRect(hdc, &paintStruct.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

                EndPaint(windowHandle, &paintStruct);
                return 0;
            }	// WM_PAINT

            case WM_CLOSE:
            {
                DestroyWindow(windowHandle);
                return 0;
            }	// WM_CLOSE

            case WM_DESTROY:
            {
                PostQuitMessage(0);

                WindowData &winData = GetAttachedWindowData(windowHandle);
                WindowCloseEvent closeEvent;
                winData.eventCallback(closeEvent);

                return 0;
            }
        }

        return DefWindowProc(windowHandle, uMsg, wParam, lParam);;
    }

    WindowsWindow::WindowData& WindowsWindow::GetAttachedWindowData(HWND windowHandle)
    {
        LONG_PTR userDataPtr = GetWindowLongPtr(windowHandle, GWLP_USERDATA);
        return *(WindowData*)userDataPtr;
    }

    #ifdef NE_PLATFORM_WINDOWS
    Nest::Window* Nest::Window::Create(const WindowProps &props)
    {
        return new WindowsWindow(props);
    }
    #endif
}