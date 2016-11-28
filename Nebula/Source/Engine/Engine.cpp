#include "Engine/Engine.h"

Engine::Engine()
{
}

Engine::Engine(const Engine& other)
{
}

Engine& Engine::operator=(const Engine& other)
{
	return *this;
}

Engine::Engine(const EngineLimits& limits)
{
	m_limits = limits;
}

Engine::~Engine()
{
	shutDown();	
}

bool Engine::startUp()
{
	m_layers.push(new SystemLayer());
	m_layers.push(new ResourceLayer());
	m_layers.push(new ModuleLayer());
	m_layers.push(new ContentLayer());

	for (unsigned int ui = 0; ui < m_layers.numElements(); ++ui)
	{
		if (m_layers.get(ui)->startUp() == LayerResponses::START_UP_FAILED)
		{
			return false;
		}
	}

	return true;
}

bool Engine::shutDown()
{
	for (unsigned int ui = 0; ui < m_layers.numElements(); ++ui)
	{
		if (m_layers.get(ui)->shutDown() == LayerResponses::SHUT_DOWN_FAILED)
		{
			return false;
		}
	}
	
	for (unsigned int ui = 0; ui < m_layers.numElements(); ++ui)
	{
		delete m_layers.get(ui);
	}
	m_layers.clear();

	return true;
}

void Engine::run()
{
}