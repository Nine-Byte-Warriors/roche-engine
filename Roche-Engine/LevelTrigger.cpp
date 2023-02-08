#include "stdafx.h"
#include "LevelTrigger.h"

LevelTrigger::LevelTrigger(const std::shared_ptr<Collider> collider)
{

	std::function<void(Collider&)> f = std::bind(&LevelTrigger::PlayerInTrigger, this, std::placeholders::_1);
	collider->AddOnEnterCallback(f);

	std::function<void(Collider&)> g = std::bind(&LevelTrigger::PlayerOutTrigger, this, std::placeholders::_1);
	collider->AddOnExitCallback(g);

	m_currentGamePhase = Phase::DayPhase;

	AddToEvent();
}
LevelTrigger::~LevelTrigger()
{
	EventSystem::Instance()->RemoveClient(EVENTID::SwapGameLevelsWindow, this);
	EventSystem::Instance()->RemoveClient(EVENTID::CurrentPhase, this);
}

void LevelTrigger::PlayerInTrigger(Collider& collider)
{
	if (collider.EntityType() == "Player"&& !m_bEventFired && m_currentGamePhase == Phase::DayPhase)
	{
			OutputDebugStringA("Change Level");
		EventSystem::Instance()->AddEvent(EVENTID::SwapGameLevelsWindow); //Change scene to game
	}
}
void LevelTrigger::PlayerOutTrigger(Collider& collider)
{
	//if (m_currentGamePhase == Phase::DayPhase && m_bEventFired)
	//{
	//	EventSystem::Instance()->AddEvent(EVENTID::CloseUIPopUp); //Change scene to game
	//}
	m_bEventFired = false;

}
void LevelTrigger::HandleEvent(Event* event)
{
	switch (event->GetEventID())
	{
	case EVENTID::SwapGameLevelsWindow:
			m_bEventFired = true;
		break;
	case EVENTID::CloseUIPopUp:
			m_bEventFired = false;
		break;
	case EVENTID::CurrentPhase:
		m_currentGamePhase = *static_cast<Phase*>(event->GetData());
		break;
	default:
		break;
	}
}

void LevelTrigger::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient(EVENTID::SwapGameLevelsWindow, this);
	EventSystem::Instance()->AddClient(EVENTID::CurrentPhase, this);
}