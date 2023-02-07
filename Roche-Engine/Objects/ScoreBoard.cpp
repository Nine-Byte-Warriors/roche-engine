#include "stdafx.h"
#include "ScoreBoard.h"

ScoreBoard::ScoreBoard()
{
	JsonLoading::LoadJson(m_scoreBoardStr, JsonFile);
	std::sort(m_scoreBoardStr.begin(), m_scoreBoardStr.end());

	AddToEvent();
}

ScoreBoard::~ScoreBoard()
{
	RemoveFromEvent();
}

void ScoreBoard::SaveScore(std::string name)
{
	ScoreBoardStr score;
	score.name = name;
	score.score = m_iScore;

	m_scoreBoardStr.push_back(score);

	std::sort(m_scoreBoardStr.begin(), m_scoreBoardStr.end());

	JsonLoading::SaveJson(m_scoreBoardStr, JsonFile);
}

int ScoreBoard::GetScore()
{
	return m_iScore;
}

std::string ScoreBoard::GetScoreStr()
{
	std::string scoreStr = "";
	for (int i = 0; i < (scoreDigitsSize - std::to_string(m_iScore).length()); i++)
	{
		scoreStr += "0";
	}
	scoreStr += std::to_string(m_iScore);

	return scoreStr;
}

std::string ScoreBoard::GetName(int num)
{
	if (num < m_scoreBoardStr.size())
	{
		return m_scoreBoardStr[num].name;
	}
	return "";
}

std::string ScoreBoard::GetScoreStr(int num)
{
	if (num >= m_scoreBoardStr.size())
		return "";

	std::string scoreStr = "";
	for (int i = 0; i < (scoreDigitsSize - std::to_string(m_scoreBoardStr[num].score).length()); i++)
	{
		scoreStr += "0";
	}
	scoreStr += std::to_string(m_scoreBoardStr[num].score);

	return scoreStr;
}

void ScoreBoard::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient(EVENTID::UpdateScore, this);
	EventSystem::Instance()->AddClient(EVENTID::ResetScore, this);
	EventSystem::Instance()->AddClient(EVENTID::SaveScore, this);
}

void ScoreBoard::RemoveFromEvent() noexcept
{
	EventSystem::Instance()->RemoveClient(EVENTID::UpdateScore, this);
	EventSystem::Instance()->RemoveClient(EVENTID::ResetScore, this);
	EventSystem::Instance()->RemoveClient(EVENTID::SaveScore, this);
}

void ScoreBoard::HandleEvent(Event* event)
{
	switch (event->GetEventID())
	{
	case EVENTID::UpdateScore:
	{
		int* score = static_cast<int*>(event->GetData());
		UpdateScore(*score);

		break;
	}
	case EVENTID::ResetScore:
	{
		ResetScore();
		break;
	}
	case EVENTID::SaveScore:
	{
		std::string* name = static_cast<std::string*>(event->GetData());
		SaveScore(*name);
		break;
	}
	default: break;
	}
}

void ScoreBoard::UpdateScore(int score)
{
	m_iScore += score;
}

void ScoreBoard::ResetScore()
{
	m_iScore = 0;
}
