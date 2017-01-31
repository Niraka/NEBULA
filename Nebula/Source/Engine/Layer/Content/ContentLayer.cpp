#include "Engine/Layer/Content/ContentLayer.h"

ContentLayer::ContentLayer()
{
}

LayerResponses ContentLayer::startLayerUp()
{
	return LayerResponses::START_UP_SUCCESS;
}

LayerResponses ContentLayer::shutLayerDown()
{
	return LayerResponses::SHUT_DOWN_SUCCESS;
}