#include <Nest.h>

class Sandbox : public Nest::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

Nest::Application* Nest::createApplication()
{
	return new Sandbox();
}