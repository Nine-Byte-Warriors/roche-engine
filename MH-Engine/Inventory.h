#pragma once
#ifndef INVENTORY_H
#define INVENTORY_H

#include <iostream>
#include <string>

using namespace std;

struct InventoryStruct
{
	string m_sSeedName;
	int m_iSeedCount;
};

class Inventory
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

	string m_currentSeed;
	int m_currentSeedIndex;
	string m_seedOptions[2];
	vector<InventoryStruct> m_seedInventory{};
	

};

#endif