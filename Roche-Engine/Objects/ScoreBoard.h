#pragma once
#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include "JsonLoading.h"

struct ScoreBoardStr
{
	std::string name;
	int score;

	bool operator < (const ScoreBoardStr& str) const
	{
		return (score > str.score);
	}
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ScoreBoardStr, name, score)

class ScoreBoard : public Listener
{
public:
	ScoreBoard();
	~ScoreBoard();

	void SaveScore(std::string name);

	int GetScore();
	std::string GetScoreStr();

	std::string GetName(int num);
	std::string GetScoreStr(int num);
private:
	void AddToEvent() noexcept;
	void RemoveFromEvent() noexcept;
	void HandleEvent(Event* event) override;

	void UpdateScore(int score);
	void ResetScore();

	int m_iScore = 0;

	std::string JsonFile = "Resources\\ScoreBoard\\ScoreBoard.json";
	std::vector<ScoreBoardStr> m_scoreBoardStr;

	int scoreDigitsSize = 7;
};

#endif