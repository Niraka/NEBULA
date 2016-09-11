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
	Start up procedure of an engine layer failed. */
	START_UP_FAILED,

	/**
	Start up procedure of an engine layer succeeded. */
	START_UP_SUCCESS,
	
	/**
	Shut down procedure of an engine layer failed. */
	SHUT_DOWN_FAILED,

	/**
	Shut down procedure of an engine layer succeeded. */
	SHUT_DOWN_SUCCESS
};

#endif