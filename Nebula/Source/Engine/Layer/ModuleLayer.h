/**
The module layer.

@date edited 10/09/2016
@date authored 10/09/2016

@author Nathan Sainsbury */

#ifndef MODULE_LAYER_H
#define MODULE_LAYER_H

#include "Engine/Layer/Layer.h"

class ModuleLayer :
	public Layer
{
	private:

	protected:

	public:
		/**
		Constructs a module layer. */
		ModuleLayer();

		/**
		Starts up the module layer.
		@return A layer response indicating the result of the operation */
		LayerResponses startUp();

		/**
		Shuts down the module layer.
		@return A layer response indicating the result of the operation */
		LayerResponses shutDown();
};

#endif