#include "Engine/Engine.h"

Engine::Engine()
{
}

Engine::Engine(const EngineLimits& limits)
{
}

bool Engine::startUp()
{
	if (m_systemLayer.startUp() == LayerResponses::START_UP_FAILED)
	{
		return false;
	}
	if (m_resourceLayer.startUp() == LayerResponses::START_UP_FAILED)
	{
		return false;
	}
	if (m_moduleLayer.startUp() == LayerResponses::START_UP_FAILED)
	{
		return false;
	}
	if (m_contentLayer.startUp() == LayerResponses::START_UP_FAILED)
	{
		return false;
	}

	return true;
}

bool Engine::shutDown()
{
	if (m_systemLayer.shutDown() == LayerResponses::SHUT_DOWN_FAILED)
	{
		return false;
	}
	if (m_resourceLayer.shutDown() == LayerResponses::SHUT_DOWN_FAILED)
	{
		return false;
	}
	if (m_moduleLayer.shutDown() == LayerResponses::SHUT_DOWN_FAILED)
	{
		return false;
	}
	if (m_contentLayer.shutDown() == LayerResponses::SHUT_DOWN_FAILED)
	{
		return false;
	}

	return true;
}

void Engine::run()
{
}