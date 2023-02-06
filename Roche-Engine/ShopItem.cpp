#include "stdafx.h"
#include "ShopItem.h"

ShopItem::ShopItem(std::shared_ptr<Collider> collider, std::string name)
{
	FilterName(name);

	std::function<void(Collider&)> f = std::bind(&ShopItem::ItemBought, this, std::placeholders::_1);
	collider->AddOnEnterCallback(f);
}

void ShopItem::ItemBought(Collider& collider)
{
	OutputDebugStringA("Collision");
	std::pair<std::string, int>* item = new std::pair<std::string, int>();
	item->first = m_itemName;
	item->second = 1;
	EventSystem::Instance()->AddEvent(EVENTID::BuySeed, item);
}

void ShopItem::HandleEvent(Event* event)
{
}

void ShopItem::AddToEvent() noexcept
{
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
