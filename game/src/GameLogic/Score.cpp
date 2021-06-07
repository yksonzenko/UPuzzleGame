#include "Score.hpp"

int Score::getScore() const
{
	return m_Score;
}

int Score::getLevel() const
{
	return m_Level;
}

int Score::getLines() const
{
	return m_Lines;
}

void Score::resetScore()
{
	m_Score = 0;
	m_Level = 1;
	m_Lines = 0;
}

void Score::addClearedLinesScore(const int& num)
{
	m_Lines += num;
	if (m_Lines > 0 && m_Lines % 10 == 0)
	{
		m_Level++;
	}
	if (num == 1)
	{
		m_Score += 40 * (m_Level);
	}
	else if (num == 2)
	{
		m_Score += 100 * (m_Level);
	}
	else if (num == 3)
	{
		m_Score += 300 * (m_Level);
	}
	else if (num == 4)
	{
		m_Score += 1200 * (m_Level);
	}
}

void Score::addBonusScore(const int& num)
{
	m_Score += num;
}
