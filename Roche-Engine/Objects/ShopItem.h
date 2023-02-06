#pragma once
#ifndef SHOPITEM_H
#define SHOPITEM_H

#include <functional>
#include "Collider.h"
#include "EventSystem.h"

class ShopItem : public Listener
{
public:
	ShopItem(const std::shared_ptr<Collider> collider, std::string name);
	~ShopItem();

	void PlayerInRange(Collider& collider);
	void PlayerOutRange(Collider& collider);

private:
	std::string m_itemName;

	bool m_bInputCheck = false;
	bool m_bCollisionCheck = false;

	// Inherited via Listener
	virtual void HandleEvent(Event* event) override;
	void AddToEvent() noexcept;

	void FilterName(std::string name);
};

#endif // !SHOPITEM_H