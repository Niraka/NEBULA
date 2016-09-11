#include "Engine/Layer/SystemLayer.h"

SystemLayer::SystemLayer()
{
}

LayerResponses SystemLayer::startUp()
{
	return LayerResponses::START_UP_SUCCESS;
}

LayerResponses SystemLayer::shutDown()
{
	return LayerResponses::SHUT_DOWN_SUCCESS;
}