#pragma once

#include <imgui.h>
#include <array>

class GameHUD
{
public:
	GameHUD();
	
	void setGameInfo(const ::std::array<int, 3>& timer, const int& score, const int& level, const int& lines);
	void drawGameInfoWindow(const bool* state);

private:
	void m_setFlags();
	void drawTimerWindow();
	void drawScoreWindow();
	void drawExtraInfoWindow();

	ImGuiWindowFlags m_WindowFlags = 0;
	
	bool m_Open;
	int m_Score{ 0 };
	int m_Level{ 0 };
	int m_Lines{ 0 };
	::std::array<int, 3> m_Timer;
};