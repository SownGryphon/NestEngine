#include <Nest.h>
#include <Nest/Core/EntryPoint.h>

#include "ProfileLayer.h"
#include "SandboxLayer.h"
#include "SandboxLayer3D.h"

class SandboxApp : public Nest::Application
{
public:
	SandboxApp() :
		Application("Sandbox Example", 1280, 720)
	{
		//getWindow().setVSync(true);
		//getWindow().setVSync(false);
		getWindow().setFPS(60);
		//pushLayer(new ProfileLayer());
		pushLayer(new SandboxLayer());
		//pushLayer(new SandboxLayer3D());
	}

	~SandboxApp()
	{

	}
};

Nest::Application* Nest::createApplication()
{
	return new SandboxApp();
}