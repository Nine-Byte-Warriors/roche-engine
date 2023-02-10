#pragma once
#ifndef INVENTORY_H
#define INVENTORY_H

class Inventory : public Listener
{
public :
	Inventory();
	~Inventory();

	inline int GetCoinCount() const noexcept { return m_iCoinAmount; }
	inline void SetCoinCount( int coins ) noexcept { m_iCoinAmount = coins; }

	inline int GetSeedPacketCount( const std::string& name ) const noexcept { return m_vSeedOptions.at(name); }
	inline int GetActiveSeedPacketCount() noexcept { return m_vSeedOptions.at(GetName()); }
	inline bool IsActiveSeedPacket( int index ) const noexcept { return m_vSelectedSeeds[index]; }
	inline int GetActiveSeedPacket() const noexcept { return m_iCurrentSeed; }
	void SetActiveSeedPacket( int currSeed );

	void LoadInventory();

	std::string GetTexture();
	std::string GetName();

private:
	void UpdateActiveSeedPacket( int currSeed );
	bool UpdateActiveSeedPacketCount();
	void UpdateCoins(int amountToChange);

	void PlantSeedFromPacket( std::string& seedName, int amountPlanted );
	void BuySeedPacket( const std::string& name, int amountBought );
	void ChangeSeedPacketValue( const std::string& name, int amountToChange );

	void AddToEvent() noexcept;
	void RemoveFromEvent() noexcept;
	void HandleEvent(Event* event) override;

	int m_iCoinAmount;
	int m_iCurrentSeed;
	// seed packet name, seed count per packet
	std::map<std::string, int> m_vSeedOptions;
	std::vector<bool> m_vSelectedSeeds = { true, false, false, false, false, false };
};

#endif