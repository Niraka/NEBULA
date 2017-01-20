#include "Engine/System/Schedule/ScheduledItem.h"

ScheduledItem::ScheduledItem()
{
	resetFlags();
}

void ScheduledItem::resetFlags()
{
	m_bRequestingSchedulerStop = false;
	m_bRequestingFrameSkip = false;
}

void ScheduledItem::onSchedulerStart(const SchedulerTimeInfo& info)
{
	return;
}

void ScheduledItem::onSchedulerStop(const SchedulerTimeInfo& info)
{
	return;
}

bool ScheduledItem::isRequestingSchedulerStop() const
{
	return m_bRequestingSchedulerStop;
}

bool ScheduledItem::isRequestingFrameSkip() const
{
	return m_bRequestingFrameSkip;
}