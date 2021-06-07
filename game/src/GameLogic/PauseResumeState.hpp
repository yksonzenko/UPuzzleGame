#pragma once

#include <MiniKit/MiniKit.hpp>
#include <array>

class PauseResume
{
public:
	PauseResume();
	
	void start(const float& deltaTime);
	void stop();
	void paused();
	void resumed();
	void resetTimer();
	void timerOn(const float& deltaTime);

	bool isResumed();
	bool isPaused();

	::std::array<int, 3> getTimer() const;

private:
	float m_ResumeTicks;
	float m_PausedTicks;
	float m_GetPassedTime;
	
	bool m_Paused;
	bool m_Resumed;

	float m_Seconds{ 0.0f };
	int m_Minutes{ 0 };
	int m_Hours{ 0 };
};