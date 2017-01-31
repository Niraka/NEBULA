#include "Engine/Engine.h"

Engine::Engine()
{
	m_layers = Vector<Layer*>(4);
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
	m_layers.push(&m_systemLayer);
	m_layers.push(&m_resourceLayer);
	m_layers.push(&m_moduleLayer);
	m_layers.push(&m_contentLayer);

	for (Layer* pLayer : m_layers)
	{
		if (pLayer->startLayerUp() == LayerResponses::START_UP_FAILED)
		{
			return false;
		}
	}

	return true;
}

bool Engine::shutDown()
{
	for (Layer* pLayer : m_layers)
	{
		if (pLayer->shutLayerDown() == LayerResponses::SHUT_DOWN_FAILED)
		{
			return false;
		}
	}
	m_layers.clear();

	return true;
}

void Engine::run()
{
	// Wont enable this for now since there is no way to stop it yet
	// m_systemLayer.startScheduler();
}