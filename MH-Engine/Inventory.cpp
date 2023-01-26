#include "stdafx.h"
#include "Inventory.h"

Inventory::Inventory()
{
	m_currentSeedIndex = 0;
	m_seedOptions[0] = {"Carrot"};
	m_seedOptions[1] = { "Tomato" };
	InventoryStruct carrot = { "Carrot",5 };
	InventoryStruct tomato = { "Tomato", 2 };
	m_seedInventory.push_back(carrot);
	m_seedInventory.push_back(tomato);
	
	m_currentSeed = m_seedOptions[m_currentSeedIndex];
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
void Inventory::UpdateCurrentSeedCount(int amountToChange)
{
	UpdateInventoryCount(m_currentSeed,amountToChange);
}

void Inventory::IncrementCurrentSeed()
{
	m_currentSeedIndex++;
	//process change code side
	if (m_currentSeedIndex >= m_seedInventory.size())
		m_currentSeedIndex = 0;


	m_currentSeed = m_seedOptions[m_currentSeedIndex];

	//apply change to ui

}

string Inventory::GetCurrentSeed()
{
	return m_currentSeed;
}


bool Inventory::SeedCountCheck(int seedIndex)
{
	if (m_seedInventory[seedIndex].m_iSeedCount >= 1)
	{
		return true;
	}

	return false;
}