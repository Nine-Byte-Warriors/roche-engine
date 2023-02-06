#include "stdafx.h"
#include "ShopItem.h"

ShopItem::ShopItem(std::shared_ptr<Collider> collider, std::string name)
{
	FilterName(name);

	std::function<void(Collider&)> f = std::bind(&ShopItem::PlayerInRange, this, std::placeholders::_1);
	collider->AddOnEnterCallback(f);

	std::function<void(Collider&)> g = std::bind(&ShopItem::PlayerOutRange, this, std::placeholders::_1);
	collider->AddOnExitCallback(g);

	AddToEvent();
}

ShopItem::~ShopItem()
{
	EventSystem::Instance()->RemoveClient(EVENTID::BuySeedAttempt, this);
}

void ShopItem::PlayerInRange(Collider& collider)
{
	m_bCollisionCheck = true;

	if (m_bInputCheck)
	{
		std::pair<std::string, int>* item = new std::pair<std::string, int>();
		item->first = m_itemName;
		item->second = 1;
		EventSystem::Instance()->AddEvent(EVENTID::BuySeed, item);
	}

	m_bInputCheck = false;
}

void ShopItem::PlayerOutRange(Collider& collider)
{
	m_bCollisionCheck = false;
}

void ShopItem::HandleEvent(Event* event)
{
	switch (event->GetEventID())
	{
	case EVENTID::BuySeedAttempt:
		if(m_bCollisionCheck)
			m_bInputCheck = true;
		break;
	default:
		break;
	}
}

void ShopItem::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient(EVENTID::BuySeedAttempt, this);
}

void ShopItem::FilterName(std::string name)
{
	if (name.contains("Carrot"))
	{
		m_itemName = "Carrot";
		return;
	}
	if (name.contains("Potato"))
	{
		m_itemName = "Potato";
		return;
	}	
	if (name.contains("Bean"))
	{
		m_itemName = "Bean";
		return;
	}	
	if (name.contains("Onion"))
	{
		m_itemName = "Onion";
		return;
	}	
	if (name.contains("Cauliflower"))
	{
		m_itemName = "Cauliflower";
		return;
	}	
	if (name.contains("Tomato"))
	{
		m_itemName = "Tomato";
		return;
	}
}
