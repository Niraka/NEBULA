#include "Engine/Layer/ModuleLayer.h"

ModuleLayer::ModuleLayer()
{
}

LayerResponses ModuleLayer::startUp()
{
	return LayerResponses::START_UP_SUCCESS;
}

LayerResponses ModuleLayer::shutDown()
{
	return LayerResponses::SHUT_DOWN_SUCCESS;
}