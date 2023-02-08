#pragma once
#ifndef HEALTH_H
#define HEALTH_H

class Collider;

class Health : public Listener
{
public:
	Health(std::string type, int entityNum, std::shared_ptr<Collider> collider);
	~Health() { RemoveFromEvent(); }

	inline float GetCurrentHealth() const noexcept { return m_fCurrentHealth; }
	inline float GetMaxHealth() const noexcept { return m_fMaxHealth; }

	void SetHealth( float maxHealth );
	void TakeDamage( float damageAmount );
	void Heal( float healAmount );
	void Hit(Collider& collider);

	void SetEntityNum(int num);

private:
	void AddToEvent() noexcept;
	void RemoveFromEvent() noexcept;
	void HandleEvent( Event* event ) override;

	int m_iEntityNum;
	std::string m_sType;
	std::shared_ptr<Collider> m_collider;
	float m_fCurrentHealth;
	float m_fMaxHealth;
};

#endif