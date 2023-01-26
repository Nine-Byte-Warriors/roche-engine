#include "stdafx.h"
#include "Inventory.h"

Inventory::Inventory()
{

}
Inventory::~Inventory()
{

}

void Inventory::UpdateInventoryCount(string seedName, int amountToChange)
{
	for (int i = 0; i < m_seedInventory.size(); i++)
	{
		if (seedName == m_seedInventory[i].m_sSeedName)
		{
			if(SeedCountCheck(i))
				m_seedInventory[i].m_iSeedCount += amountToChange;
		}
	}
}

bool Inventory::SeedCountCheck(int seedIndex)
{
	if (m_seedInventory[seedIndex].m_iSeedCount >= 1)
	{
		return true;
	}

	return false;
}