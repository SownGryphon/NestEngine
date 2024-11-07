#include <Nest.h>
#include <Nest/Core/EntryPoint.h>

#include "ProfileLayer.h"
#include "SandboxLayer.h"
#include "SandboxLayer3D.h"

class Sandbox : public Nest::Application
{
public:
	Sandbox() :
		Application("Sandbox Example", 1280, 720)
	{
		getWindow().setVSync(true);
		getWindow().setVSync(false);
		getWindow().setFPS(20);
		//pushLayer(new ProfileLayer());
		pushLayer(new SandboxLayer());
		//pushLayer(new SandboxLayer3D());
	}

	~Sandbox()
	{

	}
};

Nest::Application* Nest::createApplication()
{
	return new Sandbox();
}