#pragma once
#ifndef EVENT_H
#define EVENT_H

// Holds the list of possible events that can occur.
enum class EVENTID
{
	// User Inputs
	CharInput,
	ReadCharInput,
	ClearCharBuffer,
	MousePosition,
	ImGuiMousePosition,
	KeyInput,

	LeftMouseClick,
	LeftMouseRelease,
	RightMouseClick,
	RightMouseRelease,
	MiddleMouseClick,
	MiddleMouseRelease,

	// Level System
	SetNextLevelEvent,
	SetCurrentLevelEvent,

	// Game Events
	GamePauseEvent,
	GameUnPauseEvent,
	GameSettingsEvent,
	GameLevelChangeEvent,
	GameEndLevelEvent,
	ChangePhase,
	NextDay,
	CurrentState,
	CurrentPhase,
	CurrentDay,

	// Camera Actions
	MoveUp,
	MoveLeft,
	MoveDown,
	MoveRight,

	// Player Actions
	PlayerHealth,
	PlayerPosition,
	PlayerFire,
	PlayerDash,
	TargetPosition,
	TogglePlayerMovement,

	// Inventory
	IncrementSeedPacket,
	DecrementSeedPacket,
	PlantSeedAttempt,
	PlantSeed,
	BuySeed,

	//UI
	StartGame,
	OpenCredits,
	OpenSettings,
	GeneralTab,
	GraphicsTab,
	MusicTab,
	ControlTab,
	PauseGame, // new
	ResumeGame,
	Back, // ??

	// Utility
	WindowSizeChangeEvent,
	UpdateSettingsEvent,
	RemoveUIItemEvent,
	ShowCursorEvent,
	HideCursorEvent,
	QuitGameEvent,

	//Healthcomp
	PlayerDamage,
	PlayerHeal,
	PlayerDeath,
	EnemyDeath
};

/// <summary>
/// Used to create and handle all of the possible events in the game.
/// </summary>
class Event
{
public:
	// Constructor that sends event with data (cant be altered from the other side i believe)
	Event( EVENTID eventID, void* data ) {
		this->data = data;
		this->eventID = eventID;
	}

	// Alternate constructor to just call an event proc without data
	Event( EVENTID eventID )
	{
		this->eventID = eventID;
		this->data = nullptr;
	}

	// Destructor
	~Event() { data = nullptr; }

	// Getters
	EVENTID GetEventID() const { return eventID; }
	void* GetData() { return data; }
private:
	EVENTID eventID;
	void* data;
};

#endif