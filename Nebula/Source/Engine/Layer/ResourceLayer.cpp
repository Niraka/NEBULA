#include "Engine/Layer/ResourceLayer.h"

ResourceLayer::ResourceLayer()
{
}

LayerResponses ResourceLayer::startUp()
{
	return LayerResponses::START_UP_SUCCESS;
}

LayerResponses ResourceLayer::shutDown()
{
	return LayerResponses::SHUT_DOWN_SUCCESS;
}