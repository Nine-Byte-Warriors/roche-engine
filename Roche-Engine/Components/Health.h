#pragma once
#ifndef HEALTH_H
#define HEALTH_H

class Health
{
public:
	Health( std::string type, int entityNum )
		: m_sType( type ), m_iEntityNum( entityNum ) {}
	~Health() {}

	void SetHealth( float maxHealth );
	void TakeDamage( float damageAmount );
	void Heal( float healAmount );

private:
	int m_iEntityNum;
	std::string m_sType;
	float m_fCurrentHealth;
	float m_fMaxHealth;
};

#endif