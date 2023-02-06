#pragma once
#ifndef LEVELSTATEMACHINE_H
#define LEVELSTATEMACHINE_H

#include "EventSystem.h"
#include "LevelContainer.h"

class LevelStateMachine : public Listener
{
public:
	LevelStateMachine();
	~LevelStateMachine();
	void Render_Start();
	void Render_End();
	void Update( const float dt );

	std::shared_ptr<LevelContainer> GetCurrentLevel() const noexcept { return currentLevel; }
	std::string Add (std::shared_ptr<LevelContainer> level);
	void Remove( std::string levelName );
	void SwitchTo( std::string levelName );

private:
	void AddToEvent() noexcept;
	void RemoveFromEvent() noexcept;
	void HandleEvent( Event* event ) override;

	uint32_t insertedLevelID;
	std::shared_ptr<LevelContainer> currentLevel;
	std::map<std::string, std::shared_ptr<LevelContainer>> levels;
};

#endif