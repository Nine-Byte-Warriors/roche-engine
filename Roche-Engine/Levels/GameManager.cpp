#include "stdafx.h"
#include "GameManager.h"

void GameManager::Initialize()
{
	m_currentState = GameState::Unpaused;
	m_currentPhase = Phase::DayPhase;
	UpdateBrightness();
	EventSystem::Instance()->AddEvent(EVENTID::CurrentPhase, &m_currentPhase);
	m_currentDay = 1;
}

void GameManager::SetCurrentState(GameState state)
{
	m_currentState = state;
	EventSystem::Instance()->AddEvent(EVENTID::CurrentState, &m_currentState);
}

void GameManager::SetNextDay()
{
	m_currentDay++;

	EventSystem::Instance()->AddEvent(EVENTID::CurrentDay, &m_currentDay);
}

void GameManager::SetPhase()
{
	if (m_currentPhase == Phase::DayPhase)
		m_currentPhase = Phase::NightPhase;
	else if (m_currentPhase == Phase::NightPhase)
	{
		if (m_currentDay >= 5)
		{
			EventSystem::Instance()->AddEvent(EVENTID::WinWindow);
			return;
		}

		m_currentPhase = Phase::DayPhase;
	}

	EventSystem::Instance()->AddEvent(EVENTID::CurrentPhase, &m_currentPhase);

	if (m_currentPhase == Phase::NightPhase && m_currentDay == 5)
		EventSystem::Instance()->AddEvent(EVENTID::FinalNight);

	if (m_currentState == GameState::Unpaused) 
		EventSystem::Instance()->AddEvent(EVENTID::HUDSwap);

	UpdateBrightness();
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
	EventSystem::Instance()->RemoveClient(EVENTID::PlayDayMusic, this);
	EventSystem::Instance()->RemoveClient(EVENTID::PlayShopMusic, this);
	EventSystem::Instance()->RemoveClient(EVENTID::PlayMainMenuMusic, this);
	EventSystem::Instance()->RemoveClient(EVENTID::SetPlayerHealth, this);
	EventSystem::Instance()->RemoveClient(EVENTID::LoadPlayerHealth, this);
	EventSystem::Instance()->RemoveClient(EVENTID::UpdateBrightness, this);
	EventSystem::Instance()->RemoveClient(EVENTID::UpdateBrightness_Day, this);
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
	case EVENTID::PlayDayMusic:
		AudioEngine::GetInstance()->PlayAudio("MusicGame", "DayPhaseMusic", MUSIC);
		break;
	case EVENTID::PlayShopMusic:
		AudioEngine::GetInstance()->PlayAudio("MusicShop", "ShopMusic", MUSIC);
		break;
	case EVENTID::PlayMainMenuMusic:
		AudioEngine::GetInstance()->PlayAudio("MusicMenu", "MainMenuMusic", MUSIC);
		break;
	case EVENTID::SetPlayerHealth:
		m_fSaveCurrentHealth = *static_cast<float*>(event->GetData());
		break;
	case EVENTID::LoadPlayerHealth:
		EventSystem::Instance()->AddEvent(EVENTID::GetPlayerHealth,&m_fSaveCurrentHealth);
	case EVENTID::UpdateBrightness:
		UpdateBrightness();
		break;
	case EVENTID::UpdateBrightness_Day:
		UpdateBrightness_Day();
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
	EventSystem::Instance()->AddClient(EVENTID::PlayDayMusic, this);
	EventSystem::Instance()->AddClient(EVENTID::PlayShopMusic, this);
	EventSystem::Instance()->AddClient(EVENTID::PlayMainMenuMusic, this);
	EventSystem::Instance()->AddClient(EVENTID::SetPlayerHealth, this);
	EventSystem::Instance()->AddClient(EVENTID::LoadPlayerHealth, this);
	EventSystem::Instance()->AddClient(EVENTID::UpdateBrightness, this);
	EventSystem::Instance()->AddClient(EVENTID::UpdateBrightness_Day, this);
}

void GameManager::DayPhase()
{
	*m_fRedOverlay = 1.0f;
	*m_fGreenOverlay = 1.0f;
	*m_fBlueOverlay = 1.0f;
}

void GameManager::NightPhase()
{
	*m_fRedOverlay = 0.8f;
	*m_fGreenOverlay = 0.8f;
	*m_fBlueOverlay = 1.0f;
}

void GameManager::UpdateBrightness_Day()
{
	m_currentPhase = Phase::DayPhase;
	UpdateBrightness();
}

void GameManager::UpdateBrightness()
{
	if (m_currentPhase == Phase::DayPhase)
		DayPhase();
	else if (m_currentPhase == Phase::NightPhase)
		NightPhase();
	EventSystem::Instance()->AddEvent(EVENTID::RedOverlayColour, m_fRedOverlay);
	EventSystem::Instance()->AddEvent(EVENTID::GreenOverlayColour, m_fGreenOverlay);
	EventSystem::Instance()->AddEvent(EVENTID::BlueOverlayColour, m_fBlueOverlay);
}
