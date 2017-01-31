#include "Engine/Layer/Module/ModuleLayer.h"

ModuleLayer::ModuleLayer()
{
}

LayerResponses ModuleLayer::startLayerUp()
{
	return LayerResponses::START_UP_SUCCESS;
}

LayerResponses ModuleLayer::shutLayerDown()
{
	return LayerResponses::SHUT_DOWN_SUCCESS;
}