#pragma once

#include "Application.h"
#include "Nest/Core/Platform.h"
#include "Nest/Renderer/GraphicsAPI.h"

#include "platform/OpenGL/OpenglAPI.h"

extern Nest::Application* Nest::createApplication();

int main(int argc, char **argv)
{
	Nest::Platform::s_nestPlatform->init();

	Nest::GraphicsAPI::s_nestGraphicsAPI = Nest::createRef<Nest::OpenglAPI>();

	auto app = Nest::createApplication();
	app->run();
	delete app;
}