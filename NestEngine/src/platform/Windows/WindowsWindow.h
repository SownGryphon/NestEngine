#pragma once

#include <chrono>

#include "platform/Windows/CleanWindows.h"
#include "Nest/Core/Window.h"

namespace Nest
{
	class WindowsWindow : public Window
	{
	private:
		struct WindowData
		{
			std::string title;
			unsigned int width = 0, height = 0;
			bool resizable = false;
			bool vSync = false;
			float fps = 0.f;

			EventCallbackFn eventCallback;

			HDC deviceContext = 0;
			HGLRC openglRenderContext = 0;
		};

		std::chrono::high_resolution_clock::time_point m_lastFrameTime;
		
		HWND m_windowHandle;

		WindowData m_winData;

		inline static std::wstring s_windowClassName = L"Nest Window Class";
		inline static WNDCLASS s_windowClass = {};
	public:
		WindowsWindow(const WindowProps &props);
		virtual ~WindowsWindow();

		void processEvents() override;
		void onUpdate() override;

		unsigned int getWidth() const override;
		unsigned int getHeight() const override;

		inline void setEventCallback(const EventCallbackFn &func) override { m_winData.eventCallback = func; }
		void setVSync(bool enabled) override;
		bool isVSync() const override;
		virtual void setFPS(float fps) override;
		inline void* getNativeWindow() override { return m_windowHandle; }

		static WindowData& GetAttachedWindowData(HWND windowHandle);

	private:
		void init(const WindowProps &props);
		void shutdown();

		static LRESULT CALLBACK WindowProc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};
}