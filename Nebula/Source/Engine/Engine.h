/**
The engine.

@date edited 10/09/2016
@date authored 10/09/2016

@author Nathan Sainsbury */

#ifndef ENGINE_H
#define ENGINE_H

#include "Engine/Layer/SystemLayer.h"
#include "Engine/Layer/ResourceLayer.h"
#include "Engine/Layer/ModuleLayer.h"
#include "Engine/Layer/ContentLayer.h"
#include "Engine/EngineLimits.h"

class Engine
{
	private:
		SystemLayer m_systemLayer;
		ResourceLayer m_resourceLayer;
		ModuleLayer m_moduleLayer;
		ContentLayer m_contentLayer;

		/**
		Forbidden. */
		Engine();

	protected:

	public:
		/**
		Constructs an engine with the given limits.
		@param limits The desired limits */
		Engine(const EngineLimits& limits);
};

#endif