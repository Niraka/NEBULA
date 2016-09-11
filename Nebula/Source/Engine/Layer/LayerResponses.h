/**
Layer responses defines a series of human readable responses to operations that can be invoked on
engine layers.

@date edited 11/09/2016
@date authored 11/09/2016

@author Nathan Sainsbury */

#ifndef LAYER_RESPONSES_H
#define LAYER_RESPONSES_H

enum class LayerResponses
{
	/** 
	Initialisation of an engine layer failed. */
	INITIALISATION_FAILED,

	/**
	Initialisation of an engine layer succeeded. */
	INITIALISATION_SUCCESS
};

#endif