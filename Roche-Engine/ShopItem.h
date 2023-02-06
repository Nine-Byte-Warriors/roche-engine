#pragma once
#include <functional>
#include "Collider.h"
#include "EventSystem.h"
class ShopItem : Listener
{
public:
	ShopItem(std::shared_ptr<Collider> collider, std::string name);

	void ItemBought(Collider& collider);

private:
	std::shared_ptr<Collider> m_collider;
	std::string m_itemName;

	// Inherited via Listener
	virtual void HandleEvent(Event* event) override;
	void AddToEvent() noexcept;

	void FilterName(std::string name);
};
