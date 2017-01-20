#include "Engine/System/Schedule/Scheduler.h"

Scheduler::Scheduler()
{
	m_lastLagWarning = getTimeNanos();
	m_lagWarningInterval = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(5));

	resetExecutionData();
}

Scheduler::Scheduler(const SchedulerConfig& conf)
{
	m_activeConfig = conf;
	m_pendingConfig = m_activeConfig;

	m_lastLagWarning = getTimeNanos();
	m_lagWarningInterval = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(5));

	resetExecutionData();
}

std::chrono::nanoseconds Scheduler::getTimeNanos()
{
	std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch());
}

void Scheduler::resetExecutionData()
{
	m_executionData.uiFramesExecuted = 0;
	m_executionData.uiFramesDelayedTotal = 0;
	m_executionData.uiFramesDelayedThreadWake = 0;
	m_executionData.uiSkippedUpdateCalls = 0;
	m_executionData.uiRefusedStopRequests = 0;
}

void Scheduler::setConfig(const SchedulerConfig& config)
{
	m_pendingConfig = config;

	// 1) Check interpolation cap disable. Setting to maximum if disabled so we can bypass an
	// if-statement that checks whether interpolation capping is disabled on each iteration.
	if (m_pendingConfig.fInterpolationCap < 1.f)
	{
		m_pendingConfig.fInterpolationCap = std::numeric_limits<double>::max();
	}

	// 2) Check lag threshold isnt about to mark every frame as delayed
	if (m_pendingConfig.fInterpolationLagThreshold <= 1.f)
	{
		m_pendingConfig.fInterpolationLagThreshold = 1.01f;
	}
}

const SchedulerConfig& Scheduler::getActiveConfig() const
{
	return m_activeConfig;
}

const SchedulerConfig& Scheduler::getPendingConfig() const
{
	return m_pendingConfig;
}

const SchedulerExecutionData& Scheduler::getExecutionData() const
{
	return m_executionData;
}

void Scheduler::start()
{
	bool bUnlimited = false;
	double fFrameInterpolation = 1.f;
	std::chrono::nanoseconds timeStep;
	std::chrono::nanoseconds timeFrameRemaining;
	std::chrono::nanoseconds timeFrameStart;
	std::chrono::nanoseconds timeFrameEnd;
	std::chrono::nanoseconds timeSkipSleepThreshold;
	SchedulerTimeInfo timeInfo;

	m_bSchedulerRunning = true;
	resetExecutionData();

	// Apply pending configuration
	m_activeConfig = m_pendingConfig;
	m_lagWarningInterval = std::chrono::duration_cast<std::chrono::nanoseconds>(
		std::chrono::seconds(m_activeConfig.uiLagWarningFrequency));

	// Check if the update rate is unlimited
	if (m_activeConfig.updateRate.getTimestep().count() == m_iUnlimitedIndicator)
	{
		bUnlimited = true;
	}

	timeStep = m_activeConfig.updateRate.getTimestep();
	timeSkipSleepThreshold = std::chrono::nanoseconds((long long)(0.025 * timeStep.count()));

	// Announce start to listeners
	SchedulerEvent schedulerEvent(SchedulerEventTypes::SCHEDULER_STARTED);
	for (SchedulerListener* l : m_schedulerListeners)
	{
		l->onSchedulerEvent(schedulerEvent);
	}

	timeInfo.timeNow = getTimeNanos();
	timeInfo.timeEpoch = timeInfo.timeNow;
	timeInfo.timeSinceEpoch = std::chrono::nanoseconds(0);
	timeInfo.timeLastUpdate = timeInfo.timeNow;
	timeInfo.timeFrameStart = timeInfo.timeNow;
	timeInfo.fInterpolation = 1.0;

	// Announce start to items
	for (Pair<ScheduledItem*, SchedulerItemInfo>& schedule : m_schedules)
	{
		schedule.first->onSchedulerStart(timeInfo);
	}

	// Set the last update time to now for each schedule
	timeFrameStart = getTimeNanos();
	for (Pair<ScheduledItem*, SchedulerItemInfo>& schedule : m_schedules)
	{
		schedule.second.timeLastUpdate = timeFrameStart - schedule.second.timestep;
	}

	while (m_bSchedulerRunning)
	{
		// Calculate the frame start and end time
		timeFrameStart = getTimeNanos();
		timeFrameEnd = timeFrameStart + timeStep;

		// Configure time info structure and update each scheduled item
		timeInfo.timeFrameStart = timeFrameStart;
		for (Pair<ScheduledItem*, SchedulerItemInfo>& schedule : m_schedules)
		{
			if (schedule.first->isRequestingSchedulerStop())
			{
				if (m_activeConfig.bRefuseStopRequests)
				{
					++m_executionData.uiRefusedStopRequests;
				}
				else
				{
					schedule.first->resetFlags();
					m_bSchedulerRunning = false;
					break;	
				}
			}

			if (schedule.first->isRequestingFrameSkip())
			{
				++m_executionData.uiSkippedUpdateCalls;
				schedule.first->resetFlags();
				continue;
			}

			timeInfo.timeNow = getTimeNanos();
			timeInfo.timeSinceEpoch = timeInfo.timeNow - timeInfo.timeEpoch;
			timeInfo.timeLastUpdate = timeInfo.timeNow - schedule.second.timeLastUpdate;
			if (schedule.second.timestep.count() == m_iUnlimitedIndicator)
			{
				timeInfo.fInterpolation = 1.0;
			}
			else
			{
				timeInfo.fInterpolation = (double)(timeInfo.timeNow - schedule.second.timeLastUpdate).count()
					/ (double)schedule.second.timestep.count();
				timeInfo.fInterpolation = std::min(timeInfo.fInterpolation, m_activeConfig.fInterpolationCap);
			}
			
			if (timeFrameStart.count() > schedule.second.timeNextFrame.count() ||
				schedule.second.timestep.count() == m_iUnlimitedIndicator)
			{
				schedule.first->onUpdate(timeInfo);
				schedule.second.timeNextFrame = timeFrameStart + schedule.second.timestep;
				schedule.second.timeLastUpdate = timeFrameStart;
			}
		}

		// End frame:
		// If time left over, sleep it off
		// If not, log and potentially report the delay
		timeFrameRemaining = timeFrameEnd - getTimeNanos();
		if (timeFrameRemaining.count() > 0)
		{
			// Only sleep if more than 2.5% of the duration of the frame remains. Also trim 2.5% 
			// from expected wake-up time when sleeping. On average this makes the thread wake
			// closer to the intended time (though it is still far from perfect).
			if (timeFrameRemaining > timeSkipSleepThreshold)
			{
				std::this_thread::sleep_for(timeFrameRemaining - timeSkipSleepThreshold);
			}

			// Calculate next frame interpolation. Have to do this even when there was spare time
			// as thread waking tends to be inaccurate
			double fFrameTime = (double)(getTimeNanos().count() - timeFrameStart.count());
			fFrameInterpolation = fFrameTime / (double)timeStep.count();
			fFrameInterpolation = std::min(fFrameInterpolation, m_activeConfig.fInterpolationCap);

			// Potentially report delayed frame
			if (fFrameInterpolation > m_activeConfig.fInterpolationLagThreshold)
			{
				++m_executionData.uiFramesDelayedTotal;
				++m_executionData.uiFramesDelayedThreadWake;
			}
		}
		else
		{
			if (!bUnlimited)
			{
				// Calculate next frame interpolation
				double fFrameTime = (double)(getTimeNanos().count() - timeFrameStart.count());
				fFrameInterpolation = fFrameTime / (double)timeStep.count();
				fFrameInterpolation = std::min(fFrameInterpolation, m_activeConfig.fInterpolationCap);

				if (fFrameInterpolation > m_activeConfig.fInterpolationLagThreshold)
				{
					++m_executionData.uiFramesDelayedTotal;

					// Launch a lag event if sufficient time since last event has passed
					if (getTimeNanos().count() > (m_lastLagWarning.count() + m_lagWarningInterval.count()))
					{
						schedulerEvent = SchedulerEvent();
						schedulerEvent.type = SchedulerEventTypes::SCHEDULER_FALLING_BEHIND;
						for (SchedulerListener* l : m_schedulerListeners)
						{
							l->onSchedulerEvent(schedulerEvent);
						}

						m_lastLagWarning = getTimeNanos();
					}
				}
			}
		}

		++m_executionData.uiFramesExecuted;
	}

	timeInfo.timeNow = getTimeNanos();
	timeInfo.timeSinceEpoch = timeInfo.timeNow - timeInfo.timeEpoch;
	timeInfo.fInterpolation = 1.0;

	// Announce stop to items
	for (Pair<ScheduledItem*, SchedulerItemInfo>& schedule : m_schedules)
	{
		schedule.first->onSchedulerStop(timeInfo);
	}

	// Announce stop to listeners
	schedulerEvent = SchedulerEvent(SchedulerEventTypes::SCHEDULER_STOPPED);
	for (SchedulerListener* l : m_schedulerListeners)
	{
		l->onSchedulerEvent(schedulerEvent);
	}
}

void Scheduler::stop()
{
	m_bSchedulerRunning = false;
}

void Scheduler::addScheduledItem(ScheduledItem* const pItem, const SchedulerRate& rate)
{
	SchedulerItemInfo info;

	if (rate.getTimestep().count() == m_iUnlimitedIndicator)
	{
		info.timestep = std::chrono::nanoseconds(m_iUnlimitedIndicator);
	}
	else
	{
		info.timestep = rate.getTimestep();
	}

	if (m_bSchedulerRunning)
	{
		info.timeLastUpdate = getTimeNanos();
	}
	else
	{
		info.timeLastUpdate = std::chrono::nanoseconds(0);
	}

	info.timeNextFrame = std::chrono::nanoseconds(0);
	m_schedules.push(Pair<ScheduledItem*, SchedulerItemInfo>(pItem, info));
}

void Scheduler::removeScheduledItem(ScheduledItem* const pItem)
{
	unsigned int uiCurrentIndex = 0;
	unsigned int uiMaxIndex = m_schedules.numElements();
	for (; uiCurrentIndex < uiMaxIndex; ++uiCurrentIndex)
	{
		if (m_schedules.get(uiCurrentIndex).first == pItem)
		{
			m_schedules.removeAndReset(uiCurrentIndex);
			return;
		}
	}
}

bool Scheduler::scheduledItemExists(ScheduledItem* const pItem) const
{
	for (Pair<ScheduledItem*, SchedulerItemInfo>& pair : m_schedules)
	{
		if (pair.first == pItem)
		{
			return true;
		}
	}
	return false;
}

void Scheduler::addSchedulerListener(SchedulerListener* const pListener)
{
	m_schedulerListeners.push(pListener);
}

void Scheduler::removeSchedulerListener(SchedulerListener* const pListener)
{
	m_schedulerListeners.removeAndReset(pListener);
}

bool Scheduler::schedulerListenerExists(SchedulerListener* const pListener) const
{
	return m_schedulerListeners.exists(pListener);
}