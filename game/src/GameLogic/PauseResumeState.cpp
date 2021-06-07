#include "PauseResumeState.hpp"
#include <iostream>
#include <iomanip>

PauseResume::PauseResume() : m_ResumeTicks(0), m_PausedTicks(0), m_GetPassedTime(0.0f), m_Paused(false), m_Resumed(false) {}

void PauseResume::start(const float& deltaTime)
{
	if (!m_Paused)
	{
        m_Resumed = true;
        timerOn(deltaTime);
	}
}

void PauseResume::stop()
{
	m_Resumed = false;
	m_Paused = true;
}

void PauseResume::paused()
{
    if ((m_Resumed == true) && (m_Paused == false))
    {
        m_Paused = true;
    }
}

void PauseResume::resumed()
{
    if (m_Paused == true)
    {
        m_Paused = false;
    }
}

bool PauseResume::isPaused()
{
    return m_Paused;
}

bool PauseResume::isResumed()
{
    return m_Resumed;
}

::std::array<int,3> PauseResume::getTimer() const
{
    std::array<int, 3> timer{ m_Hours, m_Minutes, static_cast<int>(m_Seconds) };
    return timer;
}

void PauseResume::timerOn(const float& deltaTime)
{
    m_Seconds += deltaTime;
	if (m_Seconds > 59.f)
	{
        m_Seconds = 0;
        m_Minutes += 1;
	}
    if (m_Minutes > 59)
    {
        m_Minutes = 0;
        m_Hours += 1;
    }
}

void PauseResume::resetTimer()
{
    m_Seconds = 0;
    m_Minutes = 0;
    m_Hours = 0;
    this->resumed();
}
