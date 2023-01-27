#pragma once
#ifndef INVENTORY_H
#define INVENTORY_H

#include <iostream>
#include <string>
#include "EventSystem.h"

using namespace std;

struct InventoryStruct
{
	string m_sSeedName;
	int m_iSeedCount;
};

class Inventory : public Listener
{

public :
	Inventory();
	~Inventory();

	void UpdateInventoryCount(string seedName, int seedChange);
	void UpdateCurrentSeedCount(int seedChange);
	void IncrementCurrentSeed();
	string GetCurrentSeed();

private:
	bool SeedCountCheck(int seedIndex);	

public:
	void AddToEvent() noexcept;
	void HandleEvent(Event* event) override;

private:
	int m_numOfSeedOptions;
	string m_currentSeed;
	int m_currentSeedIndex;
	string m_seedOptions[6];
	vector<InventoryStruct> m_seedInventory{};
};

#endif