/**
The engine.

@date edited 28/11/2016
@date authored 10/09/2016

@author Nathan Sainsbury */

#ifndef ENGINE_H
#define ENGINE_H

#include "Engine/System/Tools/Vector.h"
#include "Engine/Layer/Layer.h"
#include "Engine/Layer/SystemLayer.h"
#include "Engine/Layer/ResourceLayer.h"
#include "Engine/Layer/ModuleLayer.h"
#include "Engine/Layer/ContentLayer.h"
#include "Engine/EngineLimits.h"

class Engine
{
	private:
		Vector<Layer*> m_layers;
		EngineLimits m_limits;

		/**
		Forbidden. Engine limits should always be given. */
		Engine();

		/**
		Forbidden. No reason to copy an entire engine. */
		Engine(const Engine& other);

		/**
		Forbidden. No reason to assign an entire engine. */
		Engine& operator=(const Engine& other);

	protected:

	public:
		/**
		Constructs an engine with the given limits.
		@param limits The desired limits */
		Engine(const EngineLimits& limits);

		/**
		Destructor. */
		~Engine();

		/**
		Starts up the engine. 
		@return True if the start up was successful, false if it was not */
		bool startUp();

		/**
		Shuts down the engine.
		@return True if the shut down was successful, false if it was not */
		bool shutDown();

		/**
		Runs the engines main loop. A successful start up is required prior to calling this
		function. */
		void run();
};

#endif