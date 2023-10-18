#pragma once

#include <string>

#include "Core.h"

namespace Nest
{
	struct WindowProps
	{
		std::string title;
		unsigned int width, height;

		WindowProps(const std::string &title = "Nest Engine", unsigned int width = 1280, unsigned int height = 720)
			: title(title), width(width), height(height) {}
	};

	class Window
	{
	public:
		virtual ~Window() {}

		virtual void onUpdate() = 0;

		virtual unsigned int getWidth() const = 0;
		virtual unsigned int getHeight() const = 0;

		virtual void setVSync(bool enabled) = 0;
		virtual bool isVSync() const = 0;

		static Window* CreateWindow(const WindowProps &props = WindowProps());
	};

}