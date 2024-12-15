#include "PlatformWindows.h"

Nest::Ref<Nest::Platform> Nest::Platform::s_nestPlatform = Nest::createRef<Nest::PlatformWindows>();

void Nest::PlatformWindows::init()
{
	s_instanceHandle = GetModuleHandle(NULL);
}