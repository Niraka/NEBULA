#include "Engine/Layer/ContentLayer.h"

ContentLayer::ContentLayer()
{
}

LayerResponses ContentLayer::startUp()
{
	return LayerResponses::START_UP_SUCCESS;
}

LayerResponses ContentLayer::shutDown()
{
	return LayerResponses::SHUT_DOWN_SUCCESS;
}