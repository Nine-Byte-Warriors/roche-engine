#pragma once
#ifndef INVENTORY_H
#define INVENTORY_H

#include "EventSystem.h"

struct UILinker
{
	int m_iSeedIndex;
	EVENTID m_event;
};

class Inventory : public Listener
{
public :
	Inventory();
	~Inventory();

	void UpdateInventoryCount(std::string seedName, int seedChange);
	void UpdateCurrentSeedCount(int seedChange);
	void SetActiveSeedPacket(int index);

private:
	bool SeedCountCheck(int seedIndex);

	void AddToEvent() noexcept;
	void RemoveFromEvent() noexcept;
	void HandleEvent(Event* event) override;

	int m_iCurrentSeed;
	std::vector<UILinker> m_UISeeds;
	// seed packet name, seed count per packet
	std::map<std::string, int> m_seedOptions;
};

#endif