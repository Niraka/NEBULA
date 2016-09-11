/**
The content layer.

@date edited 10/09/2016
@date authored 10/09/2016

@author Nathan Sainsbury */

#ifndef CONTENT_LAYER_H
#define CONTENT_LAYER_H

#include "Engine/Layer/Layer.h"

class ContentLayer :
	public Layer
{
	private:

	protected:

	public:
		/**
		Constructs a content layer. */
		ContentLayer();

		/**
		Starts up the content layer.
		@return A layer response indicating the result of the operation */
		LayerResponses startUp();

		/**
		Shuts down the content layer.
		@return A layer response indicating the result of the operation */
		LayerResponses shutDown();
};

#endif