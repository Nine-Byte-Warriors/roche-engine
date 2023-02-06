#pragma once
#include <functional>
#include "Collider.h"
#include "EventSystem.h"
class ShopItem : Listener
{
public:
	ShopItem(std::shared_ptr<Collider> collider);

	void ItemBought(Collider& collider);

private:
	std::shared_ptr<Collider> m_collider;

	// Inherited via Listener
	virtual void HandleEvent(Event* event) override;
	void AddToEvent() noexcept;
};
