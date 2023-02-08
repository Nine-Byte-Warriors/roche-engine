#pragma once
#ifndef HEALTH_H
#define HEALTH_H

class Collider;

class Health
{
public:
	Health(std::string type, int entityNum, std::shared_ptr<Collider> collider);
	~Health() {}

	inline float GetCurrentHealth() const noexcept { return m_fCurrentHealth; }
	inline float GetMaxHealth() const noexcept { return m_fMaxHealth; }

	void SetHealth( float maxHealth );
	void TakeDamage( float damageAmount );
	void Heal( float healAmount );
	void Hit(Collider& collider);

	void SetEntityNum(int num);

private:
	int m_iEntityNum;
	std::string m_sType;
	std::shared_ptr<Collider> m_collider;
	float m_fCurrentHealth;
	float m_fMaxHealth;
};

#endif