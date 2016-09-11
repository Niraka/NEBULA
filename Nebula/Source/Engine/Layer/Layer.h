/**
The engine layer base class.

@date edited 11/09/2016
@date authored 11/09/2016

@author Nathan Sainsbury */

#ifndef LAYER_H
#define LAYER_H

#include "Engine/Layer/LayerResponses.h"

class Layer
{
	private:

	protected:
		/**
		Constructs a layer. */
		Layer();

	public:
		/**
		Starts up an engine layer.
		@return A layer response indicating the result of the operation */
		virtual LayerResponses startUp() = 0;

		/**
		Shuts down an engine layer.
		@return A layer response indicating the result of the operation */
		virtual LayerResponses shutDown() = 0;
};

#endif