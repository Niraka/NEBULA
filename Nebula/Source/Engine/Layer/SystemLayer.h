/**
The system layer.

@date edited 10/09/2016
@date authored 10/09/2016

@author Nathan Sainsbury */

#ifndef SYSTEM_LAYER_H
#define SYSTEM_LAYER_H

#include "Engine/Layer/Layer.h"

class SystemLayer : 
	public Layer
{
	private:

	protected:

	public:
		/**
		Constructs a system layer. */
		SystemLayer();

		/**
		Starts up the system layer.
		@return A layer response indicating the result of the operation */
		LayerResponses startUp();

		/**
		Shuts down the system layer.
		@return A layer response indicating the result of the operation */
		LayerResponses shutDown();
};

#endif