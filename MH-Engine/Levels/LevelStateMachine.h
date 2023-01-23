#pragma once
#ifndef LEVELSTATEMACHINE_H
#define LEVELSTATEMACHINE_H

#include "EventSystem.h"
#include "LevelContainer.h"

class LevelStateMachine : public Listener
{
public:
	LevelStateMachine();
	void Render_Start();
	void Render_End();
	void Update( const float dt );

	std::shared_ptr<LevelContainer> GetCurrentLevel() const noexcept { return currentLevel; }
	uint32_t Add( std::shared_ptr<LevelContainer> level );
	void Remove( uint32_t id );
	void SwitchTo( uint32_t id );

	void AddToEvent() noexcept;
	void HandleEvent( Event* event ) override;

private:
	uint32_t insertedLevelID;
	std::shared_ptr<LevelContainer> currentLevel;
	std::unordered_map<uint32_t, std::shared_ptr<LevelContainer>> levels;
};

#endif