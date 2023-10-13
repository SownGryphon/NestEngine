#pragma once

#include "Application.h"

#ifdef NT_PLATFORM_WINDOWS

extern Nest::Application* Nest::createApplication();

int main(int argc, char **argv)
{
	auto app = Nest::createApplication();
	app->run();
	delete app;
}

#endif