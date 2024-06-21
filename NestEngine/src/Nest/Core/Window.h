#pragma once

#include <functional>
#include <string>

#include "Core.h"
#include "Nest/Events/Event.h"

namespace Nest
{
	struct WindowProps
	{
		std::string title;
		unsigned int width, height;
		bool resizable;

		WindowProps(const std::string &title = "Nest Engine", unsigned int width = 1280, unsigned int height = 720, bool resizable = false)
			: title(title), width(width), height(height), resizable(resizable) {}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void processEvents() = 0;
		virtual void onUpdate() = 0;

		virtual unsigned int getWidth() const = 0;
		virtual unsigned int getHeight() const = 0;

		virtual void setEventCallback(const EventCallbackFn &func) = 0;
		virtual void setVSync(bool enabled) = 0;
		virtual bool isVSync() const = 0;
		virtual void setFPS(float fps) = 0;
		virtual void* getNativeWindow() = 0;

		static Window* CreateWindow(const WindowProps &props = WindowProps());
	};

}