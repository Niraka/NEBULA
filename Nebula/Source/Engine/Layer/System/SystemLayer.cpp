#include "Engine/Layer/System/SystemLayer.h"

SystemLayer::SystemLayer()
{
}

LayerResponses SystemLayer::startLayerUp()
{
	// Initialise each subsystem according to their unique specifications

	return LayerResponses::START_UP_SUCCESS;
}

LayerResponses SystemLayer::shutLayerDown()
{
	// Shut down each subsystem

	return LayerResponses::SHUT_DOWN_SUCCESS;
}

void SystemLayer::setSchedulerConfig(const SchedulerConfig& config)
{
	m_scheduler.setConfig(config);
}

const SchedulerConfig& SystemLayer::getSchedulerActiveConfig() const
{
	return m_scheduler.getActiveConfig();
}

const SchedulerConfig& SystemLayer::getSchedulerPendingConfig() const
{
	return m_scheduler.getPendingConfig();
}

const SchedulerExecutionData& SystemLayer::getSchedulerExecutionData() const
{
	return m_scheduler.getExecutionData();
}

void SystemLayer::startScheduler()
{
	m_scheduler.start();
}

void SystemLayer::stopScheduler()
{
	m_scheduler.stop();
}

void SystemLayer::addScheduledItem(ScheduledItem* const pItem, const SchedulerRate& updateRate)
{
	m_scheduler.addScheduledItem(pItem, updateRate);
}

void SystemLayer::removeScheduledItem(ScheduledItem* const pItem)
{
	m_scheduler.removeScheduledItem(pItem);
}

bool SystemLayer::scheduledItemExists(ScheduledItem* const pItem) const
{
	return m_scheduler.scheduledItemExists(pItem);
}

void SystemLayer::addSchedulerListener(SchedulerListener* const pListener)
{
	m_scheduler.addSchedulerListener(pListener);
}

void SystemLayer::removeSchedulerListener(SchedulerListener* const pListener)
{
	m_scheduler.removeSchedulerListener(pListener);
}

bool SystemLayer::schedulerListenerExists(SchedulerListener* const pListener) const
{
	return m_scheduler.schedulerListenerExists(pListener);
}