#include <Nest.h>
#include <Nest/Core/EntryPoint.h>

#include "SandboxLayer.h"

class Sandbox : public Nest::Application
{
public:
	Sandbox()
		: Application("Sandbox Example", 1280, 720)
	{
		pushLayer(new SandboxLayer());
	}

	~Sandbox()
	{

	}
};

Nest::Application* Nest::createApplication()
{
	return new Sandbox();
}