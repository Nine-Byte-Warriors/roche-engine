#include "stdafx.h"
#include "LevelTrigger.h"

LevelTrigger::LevelTrigger(const std::shared_ptr<Collider> collider)
{
	std::function<void(Collider&)> f = std::bind(&LevelTrigger::PlayerInTrigger, this, std::placeholders::_1);
	collider->AddOnEnterCallback(f);

	std::function<void(Collider&)> g = std::bind(&LevelTrigger::PlayerOutTrigger, this, std::placeholders::_1);
	collider->AddOnExitCallback(g);

	AddToEvent();
}
LevelTrigger::~LevelTrigger()
{
	//EventSystem::Instance()->RemoveClient(EVENTID::SwapGameLevelsWindow, this);
}

void LevelTrigger::PlayerInTrigger(Collider& collider)
{
	if (collider.EntityType() != "Player")
	{
		return;
	}
	if (!m_bEventFired)
	{
		OutputDebugStringA("Change Level");
		m_bEventFired = true;
	}
	//EventSystem::Instance()->AddEvent(EVENTID::SwapGameLevelsWindow); //Change scene to game
}
void LevelTrigger::PlayerOutTrigger(Collider& collider)
{
	//m_bEventFired = false;
}
void LevelTrigger::HandleEvent(Event* event)
{
	switch (event->GetEventID())
	{
	//case EVENTID::SwapGameLevelsWindow:
	//		m_bEventFired = true;
	//	break;
	case EVENTID::CloseUIPopUp:
			m_bEventFired = false;
		break;
	default:
		break;
	}
}

void LevelTrigger::AddToEvent() noexcept
{
	//EventSystem::Instance()->AddClient(EVENTID::SwapGameLevelsWindow, this);
}