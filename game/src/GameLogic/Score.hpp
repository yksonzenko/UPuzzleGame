#pragma once

class Score
{
public:
	int getScore() const;
	int getLevel() const;
	int getLines() const;
	void resetScore();
	void addClearedLinesScore(const int& num);
	void addBonusScore(const int& num);

private:
	int m_Score{ 0 };
	int m_Level{ 1 };
	int m_Lines{ 0 };
};