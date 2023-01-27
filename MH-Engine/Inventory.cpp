#include "stdafx.h"
#include "Inventory.h"

Inventory::Inventory()
{
	AddToEvent();
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
	EventSystem::Instance()->RemoveClient(EVENTID::UpdateInventory, this);
	EventSystem::Instance()->RemoveClient(EVENTID::ChangeSeed, this);
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
	OutputDebugStringA("Change seed");
	//apply change to ui

}

string Inventory::GetCurrentSeed()
{
	return m_currentSeed;
}

void Inventory::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient(EVENTID::UpdateInventory, this);
	EventSystem::Instance()->AddClient(EVENTID::ChangeSeed, this);
}

void Inventory::HandleEvent(Event* event)
{
	switch (event->GetEventID())
	{
	case EVENTID::UpdateInventory: UpdateCurrentSeedCount(-1); break;
	case EVENTID::ChangeSeed: IncrementCurrentSeed(); break;
	default: break;
	}
}

bool Inventory::SeedCountCheck(int seedIndex)
{
	if (m_seedInventory[seedIndex].m_iSeedCount >= 1)
	{
		return true;
	}
	OutputDebugStringA("No seed");
	return false;
}