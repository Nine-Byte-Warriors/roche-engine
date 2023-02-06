#include "stdafx.h"
#include "ScoreBoard.h"

ScoreBoard::ScoreBoard()
{
	AddToEvent();
}

ScoreBoard::~ScoreBoard()
{
	RemoveFromEvent();
}

void ScoreBoard::SaveScore(std::string name)
{
	JsonLoading::LoadJson(m_scoreBoardStr, JsonFile);

	ScoreBoardStr score;
	score.name = name;
	score.score = m_iScore;
	m_scoreBoardStr.push_back(score);

	JsonLoading::SaveJson(m_scoreBoardStr, JsonFile);
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
