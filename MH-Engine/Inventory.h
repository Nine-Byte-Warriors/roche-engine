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
private:

	bool SeedCountCheck(int seedIndex);

	string m_currentSeed;
	vector<InventoryStruct> m_seedInventory ;

};

#endif