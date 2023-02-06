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
	//std::pair<>
	//EventSystem::Instance()->AddEvent(EVENTID::BuySeed, );
}

void ShopItem::HandleEvent(Event* event)
{
}

void ShopItem::AddToEvent() noexcept
{
}
