#include <Nest.h>
#include <Nest/Core/EntryPoint.h>

#include "SandboxLayer.h"
//#include "NEATBoidLayer.h"
#include "EvolutionSimLayer.h"

class Sandbox : public Nest::Application
{
public:
	Sandbox()
		: Application("Sandbox Example", 1280, 720)
	{
		pushLayer(new SandboxLayer());
		//pushLayer(new NEATBoidLayer({ 0, 0, 1280 * 5, 720 * 5 }));
		//pushLayer(new EvolutionSimLayer(10000, 8000, 1280.f / 720.f));
	}

	~Sandbox()
	{

	}
};

Nest::Application* Nest::createApplication()
{
	return new Sandbox();
}