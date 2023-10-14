#pragma once

#ifdef NE_PLATFORM_WINDOWS

#include "Application.h"

extern Nest::Application* Nest::createApplication();

int main(int argc, char **argv)
{
	auto app = Nest::createApplication();
	app->run();
	delete app;
}

#endif