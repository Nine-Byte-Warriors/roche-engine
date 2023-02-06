#include "stdafx.h"
#include "ShopItem.h"

ShopItem::ShopItem(std::shared_ptr<Collider> collider)
{
	std::function<void(Collider&)> f = std::bind(&ShopItem::ItemBought, this, std::placeholders::_1);
	collider->AddOnEnterCallback(f);
}

void ShopItem::ItemBought(Collider& collider)
{
	OutputDebugStringA("Collision");
	std::pair<std::string, int>* item = new std::pair<std::string, int>();
	item->first = "Carrot";
	item->second = 1;
	EventSystem::Instance()->AddEvent(EVENTID::BuySeed, item);
}

void ShopItem::HandleEvent(Event* event)
{
}

void ShopItem::AddToEvent() noexcept
{
}
