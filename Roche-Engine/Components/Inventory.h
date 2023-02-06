#pragma once
#ifndef INVENTORY_H
#define INVENTORY_H

#include "EventSystem.h"

class Inventory : public Listener
{
public :
	Inventory();
	~Inventory();

	inline int GetActiveSeedPacketCount( const std::string& name ) const noexcept { return m_vSeedOptions.at(name); }
	inline bool IsActiveSeedPacket( int index ) const noexcept { return m_vSelectedSeeds[index]; }
	inline int GetActiveSeedPacket() const noexcept { return m_iCurrentSeed; }
	void SetActiveSeedPacket( int currSeed );

	std::string GetTexture();
	std::string GetName();

private:
	void UpdateActiveSeedPacket( int currSeed );
	bool UpdateActiveSeedPacketCount();

	void PlantSeedFromPacket( std::string& seedName, int amountPlanted );
	void BuySeedPacket( const std::string& name, int amountBought );
	void ChangeSeedPacketValue( const std::string& name, int amountToChange );

	void AddToEvent() noexcept;
	void RemoveFromEvent() noexcept;
	void HandleEvent(Event* event) override;

	void UpdateCoins();

	std::string GetKey();

	int m_iCurrentSeed;
	// seed packet name, seed count per packet
	std::map<std::string, int> m_vSeedOptions;
	std::vector<bool> m_vSelectedSeeds = { true, false, false, false, false, false };

	int m_iCoinAmount;
	int CoinGain;
};

#endif