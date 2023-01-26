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



private:
	void SetJsonFile(const string& name);
	void SaveInventory();

	void UpdateInventoryCount(string seedName, int seedChange);
	bool SeedCountCheck(int seedIndex);

	string m_currentSeed;
	vector<InventoryStruct> m_seedInventory;

};

#endif