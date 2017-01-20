#include "Engine/System/Schedule/SchedulerRate.h"

SchedulerRate::SchedulerRate()
{
	m_timestep = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(1)) / 60;
}

SchedulerRate::SchedulerRate(int iCount, SchedulerIntervals interval)
{
	set(iCount, interval);
}

SchedulerRate::SchedulerRate(SchedulerRatePresets preset)
{
	set(preset);
}

void SchedulerRate::set(int iCount, SchedulerIntervals interval)
{
	switch (interval)
	{
	case SchedulerIntervals::PER_HOUR:
		m_timestep = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::hours(1)) / iCount;
		break;
	case SchedulerIntervals::PER_MINUTE:
		m_timestep = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::minutes(1)) / iCount;
		break;
	case SchedulerIntervals::PER_SECOND:
		m_timestep = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(1)) / iCount;
		break;
	case SchedulerIntervals::PER_MILLISECOND:
		m_timestep = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::milliseconds(1)) / iCount;
		break;
	default:
		m_timestep = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(1)) / 60;
		break;
	}
}

void SchedulerRate::set(SchedulerRatePresets preset)
{
	switch (preset)
	{
	case SchedulerRatePresets::_30_PER_SECOND:
		m_timestep = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(1)) / 30;
		break;
	case SchedulerRatePresets::_60_PER_SECOND:
		m_timestep = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(1)) / 60;
		break;
	case SchedulerRatePresets::_90_PER_SECOND:
		m_timestep = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(1)) / 90;
		break;
	case SchedulerRatePresets::_120_PER_SECOND:
		m_timestep = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(1)) / 120;
		break;
	case SchedulerRatePresets::UNLIMITED:
		m_timestep = std::chrono::nanoseconds(-1);
		break;
	default:
		m_timestep = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(1)) / 60;
		break;
	}
}

const std::chrono::nanoseconds& SchedulerRate::getTimestep() const
{
	return m_timestep;
}