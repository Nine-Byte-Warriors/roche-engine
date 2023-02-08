#pragma once
#ifndef ENTITYCONTROLLER_H
#define ENTITYCONTROLLER_H

#include "JsonLoading.h"
#include "EntityAnimation.h"
#include "EventSystem.h"

struct EntityData
{
	std::string name;
	std::string texture;
	std::string type;
	std::vector<float> position;
	std::vector<float> scale;
	float rotation;
	float health;
	int identifier;
	std::vector<int> maxFrame;
	float mass;
	float speed;
	std::string behaviour;
	std::string colliderShape;
	std::vector<float> colliderRadius;
	bool bColliderTrigger;
	bool bColliderEnabled;
	bool bColliderStatic;
	std::string sColliderLayer;
	bool bColliderInteractDecoration;
	bool bColliderInteractPlayer;
	bool bColliderInteractEnemy;
	bool bColliderInteractProjectile;
	std::string projectilePattern;
	std::string projectileBullet;
	bool AI;
	bool projectileSystem;
	bool collider;
	bool bProjectilePattern;
	bool bProjectileBullet;
	bool animation;
	std::string animationPath;
	std::string animationType;
	int rows;
	int columns;
	bool audio;
	std::string soundBankName;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(EntityData,
	name,
	texture,
	type,
	position,
	scale,
	rotation,
	health,
	identifier,
	maxFrame,
	mass,
	speed,
	behaviour,
	colliderShape,
	colliderRadius,
	bColliderTrigger,
	bColliderEnabled,
	bColliderStatic,
	sColliderLayer,
	bColliderInteractDecoration,
	bColliderInteractPlayer,
	bColliderInteractEnemy,
	bColliderInteractProjectile,
	projectilePattern,
	projectileBullet,
	AI,
	projectileSystem,
	collider,
	bProjectilePattern,
	bProjectileBullet,
	animationPath,
	animationType,
	animation,
	rows,
	columns,
	audio,
	soundBankName)

class EntityController : public Listener
{
public:
	EntityController();
	~EntityController();
	
	void SetJsonFile( const std::string& name );
	int GetSize();

	int GetEntityNumFromName(std::string name);
	int GetEntityEnemyNumFromName(std::string name);

	std::string GetName(int num);
	std::string GetType(int num);
	std::string GetTexture(int num);
	std::vector<float> GetPosition(int num);
	float GetRotation(int num);
	float GetHealth(int num);
	std::vector<float> GetScale(int num);
	std::vector<int> GetMaxFrame(int num);
	std::string GetAnimationFile(int num);
	std::string GetAnimationType(int num);

	Vector2f GetEnemyWidthHeight(int num);

	int GetRows(int num);
	int GetColumns(int num);

	float GetMass(int num);
	float GetSpeed(int num);

	std::string GetBehaviour(int num);

	std::string GetColliderShape(int num);
	std::vector<float> GetColliderRadius(int num);
	bool GetColliderTrigger(int num);
	bool GetColliderEnabled(int num);
	std::string GetColliderLayer(int num);
	bool GetColliderStatic(int num);
	std::vector<bool> GetColliderMask(int num);

	EntityData* GetProjectileBullet(int num);

	std::string GetSoundBankName(int num);

	void SetEntityData(std::vector<EntityData> entityData);
	void AddEntityData(std::vector<EntityData> entityData);
	void AddEntityData(EntityData entityData);

	bool HasAI(int num);
	bool HasProjectileSystem(int num);
	bool HasCollider(int num);
	bool HasAnimation(int num);

	bool HasProjectileBullet(int num);
	bool HasProjectilePattern(int num);
	inline const std::string GetProjectilePattern(const int iEntityNum) { return m_entityData[iEntityNum].projectilePattern; }
	inline void SetProjectilePattern(const int iEntityNum, const std::string sPatternFile) { m_entityData[iEntityNum].projectilePattern = sPatternFile; }

	bool HasAudio(int num);

	bool HasComponentUpdated();

	void UpdateCopy();

	void SetDead(int num);
	void ClearDead();
	std::vector<int> GetDead();

private:
	void RemoveEnemiesFromEntityData();
	void RemoveNonEnemiesFromEntityEnemyData();
	
	// Inherited via Listener
	void HandleEvent(Event* event) override;
	void AddToEvent() noexcept;

	std::string JsonFile = "Entity.json";

	std::vector<EntityData> m_entityEnemyData;
	std::vector<EntityData> m_entityData;
	std::vector<EntityData> m_entityDataCopy;
	bool m_bComponentUpdated = false;
	std::vector<int> m_dead;
};

#endif