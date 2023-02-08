 #include "stdafx.h"
#include "GameManager.h"

void GameManager::Initialize()
{
	m_currentState = GameState::Unpaused;
	m_currentPhase = Phase::DayPhase;
	m_currentDay = 1;
}

void GameManager::SetCurrentState(GameState state)
{
	m_currentState = state;
	EventSystem::Instance()->AddEvent(EVENTID::CurrentState, &m_currentState);
}

void GameManager::SetNextDay()
{
	if (m_currentDay <= 5)
		m_currentDay++;
	else
		m_currentDay = 1;

	EventSystem::Instance()->AddEvent(EVENTID::CurrentDay, &m_currentDay);
}

void GameManager::SetPhase()
{
	if (m_currentPhase == Phase::DayPhase)
	{
		m_currentPhase = Phase::NightPhase;
	}
	else if (m_currentPhase == Phase::NightPhase)
	{
		m_currentPhase = Phase::DayPhase;
	}
	EventSystem::Instance()->AddEvent(EVENTID::CurrentPhase, &m_currentPhase);
	if (m_currentState == GameState::Unpaused) {
		EventSystem::Instance()->AddEvent(EVENTID::HUDSwap);
	}
}

GameManager::~GameManager()
{
	EventSystem::Instance()->RemoveClient(EVENTID::GameLevelChangeEvent, this);
	EventSystem::Instance()->RemoveClient(EVENTID::ChangePhase, this);
	EventSystem::Instance()->RemoveClient(EVENTID::GetPhase, this);
	EventSystem::Instance()->RemoveClient(EVENTID::NextDay, this);
	EventSystem::Instance()->RemoveClient(EVENTID::GameRestartEvent, this);
	EventSystem::Instance()->RemoveClient(EVENTID::PauseGame, this);
	EventSystem::Instance()->RemoveClient(EVENTID::ResumeGame, this);
}

void GameManager::HandleEvent(Event* event)
{
	switch (event->GetEventID())
	{
	case EVENTID::GameLevelChangeEvent:
		SetCurrentState(GameState::Unpaused);
		break;
	case EVENTID::ChangePhase:
		SetPhase();
		break;

	case EVENTID::NextDay:
		SetNextDay();
		break;
	case EVENTID::GameRestartEvent:
		Initialize(); // reinitialize game manager
		break;
	case EVENTID::PauseGame:
		if (m_currentState == GameState::Unpaused) {
			SetCurrentState(GameState::Paused);
		}
		break;
	case EVENTID::ResumeGame:
		if (m_currentState == GameState::Paused) {
			SetCurrentState(GameState::Unpaused);
		}
		break;
	default:
		break;
	}
}

void GameManager::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient(EVENTID::GameLevelChangeEvent, this);
	EventSystem::Instance()->AddClient(EVENTID::ChangePhase, this);
	EventSystem::Instance()->AddClient(EVENTID::GetPhase, this);
	EventSystem::Instance()->AddClient(EVENTID::NextDay, this);
	EventSystem::Instance()->AddClient(EVENTID::GameRestartEvent, this);
	EventSystem::Instance()->AddClient(EVENTID::PauseGame, this);
	EventSystem::Instance()->AddClient(EVENTID::ResumeGame, this);
}
