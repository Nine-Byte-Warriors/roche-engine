#pragma once
#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include "EventSystem.h"
#include "JsonLoading.h"

struct ScoreBoardStr
{
	std::string name;
	int score;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ScoreBoardStr, name, score)

class ScoreBoard : public Listener
{
public:
	ScoreBoard();
	~ScoreBoard();

	void SaveScore(std::string name);
private:
	void AddToEvent() noexcept;
	void RemoveFromEvent() noexcept;
	void HandleEvent(Event* event) override;

	void UpdateScore(int score);
	void ResetScore();

	int m_iScore = 0;

	std::string JsonFile = "Resources\\ScoreBoard\\ScoreBoard.json";
	std::vector<ScoreBoardStr> m_scoreBoardStr;
};

#endif