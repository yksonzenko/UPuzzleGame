#include "GameHUD.hpp"

GameHUD::GameHUD() : m_Open(false), m_Timer()
{
    this->m_setFlags();
}

void GameHUD::m_setFlags()
{
    static bool no_titlebar = true;
    static bool no_scrollbar = true;
    static bool no_menu = true;
    static bool no_move = true;
    static bool no_resize = true;
    static bool no_collapse = true;
    static bool no_close = false;
    static bool no_nav = true;
    static bool no_background = true;
    static bool no_bring_to_front = true;

    if (no_titlebar)        m_WindowFlags |= ImGuiWindowFlags_NoTitleBar;
    if (no_scrollbar)       m_WindowFlags |= ImGuiWindowFlags_NoScrollbar;
    if (!no_menu)           m_WindowFlags |= ImGuiWindowFlags_MenuBar;
    if (no_move)            m_WindowFlags |= ImGuiWindowFlags_NoMove;
    if (no_resize)          m_WindowFlags |= ImGuiWindowFlags_NoResize;
    if (no_collapse)        m_WindowFlags |= ImGuiWindowFlags_NoCollapse;
    if (no_nav)             m_WindowFlags |= ImGuiWindowFlags_NoNav;
    if (no_background)      m_WindowFlags |= ImGuiWindowFlags_NoBackground;
    if (no_bring_to_front)  m_WindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
    if (no_close)           m_Open = NULL;
}



void GameHUD::setGameInfo(const ::std::array<int, 3>& timer, const int& score, const int& level, const int& lines)
{
    m_Timer = timer;
	m_Score = score;
	m_Level = level;
	m_Lines = lines;
}

void GameHUD::drawTimerWindow()
{
    ::ImGui::SetNextWindowPos(ImVec2(700, 85), 0);
    ::ImGui::SetNextWindowSize(ImVec2(180, 60), 0);
    ImGuiStyle* styleTime = &ImGui::GetStyle();
    styleTime->WindowPadding = ImVec2(70, 0);
    styleTime->WindowRounding = 2.0f;

    ::ImGui::Begin("Time Info", &m_Open, m_WindowFlags);
    ::ImGui::Text("  Time:\n");

    if (m_Timer.at(0) < 10)
    {
        ::ImGui::Text("0%i:", m_Timer.at(0));
    }
    else
    {
        ::ImGui::SameLine(0.0f, 0.0f);
        ::ImGui::Text("%i:", m_Timer.at(0));
    }
    if (m_Timer.at(1) < 10)
    {
        ::ImGui::SameLine(0.0f, 0.0f);
        ::ImGui::Text("0%i:", m_Timer.at(1));
    }
    else
    {
        ::ImGui::SameLine(0.0f, 0.0f);
        ::ImGui::Text("%i:", m_Timer.at(1));
    }
    if (m_Timer.at(2) < 10)
    {
        ::ImGui::SameLine(0.0f, 0.0f);
        ::ImGui::Text("0%i", m_Timer.at(2));
    }
    else
    {
        ::ImGui::SameLine(0.0f, 0.0f);
        ::ImGui::Text("%i", m_Timer.at(2));
    }
    ::ImGui::End();
}

void GameHUD::drawScoreWindow()
{
    ::ImGui::SetNextWindowPos(ImVec2(60, 65), 0);
    ::ImGui::SetNextWindowSize(ImVec2(180, 165), 0);
    ImGuiStyle* styleScore = &ImGui::GetStyle();
    styleScore->WindowPadding = ImVec2(70, 0);
    styleScore->WindowRounding = 2.0f;

    ::ImGui::Begin("Score/Level/Lines Info", &m_Open, m_WindowFlags);
    ::ImGui::Text("\n\nScore: \n  %i", m_Score);
    ::ImGui::Text("\nLevel: \n  %i", m_Level);
    ::ImGui::Text("\nLines: \n  %i", m_Lines);
    ::ImGui::End();
}

void GameHUD::drawExtraInfoWindow()
{
    ::ImGui::SetNextWindowPos(ImVec2(80, 680), 0);
    ::ImGui::SetNextWindowSize(ImVec2(180, 60), 0);
    ImGuiStyle* styleExtra = &ImGui::GetStyle();
    styleExtra->WindowPadding = ImVec2(10, 0);
    styleExtra->WindowRounding = 2.0f;
    ::ImGui::Begin("Exit", &m_Open, m_WindowFlags);
    ::ImGui::Text("Press ESC to exit.");
    ::ImGui::End();
}

void GameHUD::drawGameInfoWindow(const bool* state)
{
    m_Open = state;

    drawScoreWindow();
    drawTimerWindow();
    drawExtraInfoWindow();
}


