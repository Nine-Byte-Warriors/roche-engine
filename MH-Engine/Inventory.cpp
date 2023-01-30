#include "stdafx.h"
#include "Inventory.h"

Inventory::Inventory()
{
	AddToEvent();
	m_seedOptions.push_back("Bean");
	m_seedOptions.push_back("Carrot");
	m_seedOptions.push_back("Cauliflower");
	m_seedOptions.push_back("Onion");
	m_seedOptions.push_back("Potato");
	m_seedOptions.push_back("Tomato");

	m_numOfSeedOptions = m_seedOptions.size();
	m_currentSeedIndex = 0;
	m_currentSeed = m_seedOptions[m_currentSeedIndex];

	for (int i = 0; i < m_numOfSeedOptions; i++)
	{
		m_seedInventory.push_back({ m_seedOptions[i],0 });
	}
}
Inventory::~Inventory()
{
	EventSystem::Instance()->RemoveClient(EVENTID::PlantSeed, this);
	EventSystem::Instance()->RemoveClient(EVENTID::BuySeed, this);
	EventSystem::Instance()->RemoveClient(EVENTID::ChangeSeed, this);
}

void Inventory::UpdateInventoryCount(string seedName, int amountToChange)
{
	for (int i = 0; i < m_numOfSeedOptions; i++)
	{
		if (seedName == m_seedInventory[i].m_sSeedName)
		{
			if (amountToChange > 0)
			{
				m_seedInventory[i].m_iSeedCount += amountToChange;
				OutputDebugStringA("Seed Added ");
			}
			else if(SeedCountCheck(i))
			{
				m_seedInventory[i].m_iSeedCount += amountToChange;
				OutputDebugStringA("Seed removed ");
			}
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
	OutputDebugStringA("Change seed\n");
	//apply change to ui

}

string Inventory::GetCurrentSeed()
{
	return m_currentSeed;
}

bool Inventory::SeedCountCheck(int seedIndex)
{
	if (m_seedInventory[seedIndex].m_iSeedCount> 0)
	{
		return true;
	}
	OutputDebugStringA("No seed ");
	return false;
}

void Inventory::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient(EVENTID::PlantSeed, this);
	EventSystem::Instance()->AddClient(EVENTID::BuySeed, this);
	EventSystem::Instance()->AddClient(EVENTID::ChangeSeed, this);
}

void Inventory::HandleEvent(Event* event)
{
	switch (event->GetEventID())
	{
	case EVENTID::PlantSeed: UpdateCurrentSeedCount(-1); break;
	case EVENTID::BuySeed: UpdateCurrentSeedCount(1); break;
	case EVENTID::ChangeSeed: IncrementCurrentSeed(); break;
	default: break;
	}
}