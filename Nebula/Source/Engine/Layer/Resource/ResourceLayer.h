/**
The resource layer.

@date edited 10/09/2016
@date authored 10/09/2016

@author Nathan Sainsbury */

#ifndef RESOURCE_LAYER_H
#define RESOURCE_LAYER_H

#include "Engine/Layer/Layer.h"

class ResourceLayer :
	public Layer
{
	private:

	protected:

	public:
		/**
		Constructs a resource layer. */
		ResourceLayer();

		/**
		Starts up the resource layer.
		@return A layer response indicating the result of the operation */
		LayerResponses startLayerUp();

		/**
		Shuts down the resource layer.
		@return A layer response indicating the result of the operation */
		LayerResponses shutLayerDown();
};

#endif