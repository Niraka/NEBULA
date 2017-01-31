#include "Engine/Layer/Resource/ResourceLayer.h"

ResourceLayer::ResourceLayer()
{
}

LayerResponses ResourceLayer::startLayerUp()
{
	return LayerResponses::START_UP_SUCCESS;
}

LayerResponses ResourceLayer::shutLayerDown()
{
	return LayerResponses::SHUT_DOWN_SUCCESS;
}