#include "stdafx.h"
#include "Inventory.h"

Inventory::Inventory()
{
	AddToEvent();
	m_vSeedOptions.emplace( std::pair<std::string, int>{ "Carrot", 0 } );
	m_vSeedOptions.emplace( std::pair<std::string, int>{ "Bean", 0 } );
	m_vSeedOptions.emplace( std::pair<std::string, int>{ "Onion", 0 } );
	m_vSeedOptions.emplace( std::pair<std::string, int>{ "Cauliflower", 0 } );
	m_vSeedOptions.emplace( std::pair<std::string, int>{ "Potato", 0 } );
	m_vSeedOptions.emplace( std::pair<std::string, int>{ "Tomato", 0 } );
	m_iCurrentSeed = 0;
}

Inventory::~Inventory() { RemoveFromEvent(); }

void Inventory::SetActiveSeedPacket( int currSeed )
{
	m_iCurrentSeed = currSeed;
	std::fill( m_vSelectedSeeds.begin(), m_vSelectedSeeds.end(), false );
	m_vSelectedSeeds[m_iCurrentSeed] = true;
}

//void Inventory::UpdateInventoryCount(std::string seedName, int amountToChange)
//{
//	int index = 0;
//	for (auto& [key, value] : m_seedOptions)
//	{
//		if (seedName == key)
//		{
//			value += amountToChange;
//
//			//if (amountToChange > 0)
//			//	value += amountToChange;
//			//else if(SeedCountCheck(index))
//			//	value += amountToChange;
//		}
//		index++;
//	}
//}

//void Inventory::UpdateCurrentSeedCount(int amountToChange)
//{
//	auto it = m_seedOptions.begin();
//	std::advance(it, m_iCurrentSeed);
//	UpdateInventoryCount(it->first,amountToChange);
//}

void Inventory::UpdateActiveSeedPacket( int currSeed )
{
	if ( currSeed > m_vSeedOptions.size() - 1 )
		m_iCurrentSeed = 0;
	if ( currSeed < 0 )
		m_iCurrentSeed = m_vSeedOptions.size() - 1;
	SetActiveSeedPacket( m_iCurrentSeed );
}

//bool Inventory::SeedCountCheck(int seedIndex)
//{
//	auto it = m_seedOptions.begin();
//	std::advance(it, seedIndex);
//	if (it->second > 0)
//		return true;
//	return false;
//}

void Inventory::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient(EVENTID::IncrementSeedPacket, this);
	EventSystem::Instance()->AddClient(EVENTID::DecrementSeedPacket, this);
	//EventSystem::Instance()->AddClient(EVENTID::PlantSeed, this);
	//EventSystem::Instance()->AddClient(EVENTID::BuySeed, this);
}

void Inventory::RemoveFromEvent() noexcept
{
	EventSystem::Instance()->RemoveClient(EVENTID::IncrementSeedPacket, this);
	EventSystem::Instance()->RemoveClient(EVENTID::DecrementSeedPacket, this);
	//EventSystem::Instance()->RemoveClient(EVENTID::PlantSeed, this);
	//EventSystem::Instance()->RemoveClient(EVENTID::BuySeed, this);
}

void Inventory::HandleEvent(Event* event)
{
	switch (event->GetEventID())
	{
	case EVENTID::IncrementSeedPacket:
		m_iCurrentSeed++;
		UpdateActiveSeedPacket( m_iCurrentSeed );
		break;
	case EVENTID::DecrementSeedPacket:
		m_iCurrentSeed--;
		UpdateActiveSeedPacket( m_iCurrentSeed );
		break;
	//case EVENTID::PlantSeed: UpdateCurrentSeedCount(-1); break;
	//case EVENTID::BuySeed: UpdateCurrentSeedCount(1); break;
	default: break;
	}
}