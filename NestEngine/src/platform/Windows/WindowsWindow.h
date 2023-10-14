#pragma once

#include "../../Nest/Window.h"

namespace Nest
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps &props);
		virtual ~WindowsWindow();

		void onUpdate() override;

		unsigned int getWidth() const override;
		unsigned int getHeight() const override;
	private:
		void Init();
		void Shutdown();
	};
}