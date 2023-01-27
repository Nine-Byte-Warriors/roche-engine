#include "stdafx.h"
#include "Inventory.h"

Inventory::Inventory()
{
	AddToEvent();
	
	m_seedOptions[0] = {"Bean"};
	m_seedOptions[1] = { "Carrot" };
	m_seedOptions[2] = { "Cauliflower" };
	m_seedOptions[3] = { "Onion" };
	m_seedOptions[4] = { "Potato" };
	m_seedOptions[5] = { "Tomato" };

	for (int i = 0; i < 6; i++)
	{
		m_seedInventory.push_back({m_seedOptions[i],0});
	}
				//Debug use only//
	m_seedInventory[1].m_iSeedCount = 5;
	m_seedInventory[5].m_iSeedCount = 2;
	//									//
	m_numOfSeedOptions = m_seedInventory.size();
	m_currentSeedIndex = 0;
	m_currentSeed = m_seedOptions[m_currentSeedIndex];
	
}
Inventory::~Inventory()
{
	EventSystem::Instance()->RemoveClient(EVENTID::UpdateInventory, this);
	EventSystem::Instance()->RemoveClient(EVENTID::ChangeSeed, this);
}

void Inventory::UpdateInventoryCount(string seedName, int amountToChange)
{
	for (int i = 0; i < m_numOfSeedOptions; i++)
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
	//process change code side
	m_currentSeedIndex++;
	if (m_currentSeedIndex >= m_numOfSeedOptions)
		m_currentSeedIndex = 0;

	m_currentSeed = m_seedOptions[m_currentSeedIndex];
	OutputDebugStringA("Change seed");
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
	OutputDebugStringA("No seed");
	return false;
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