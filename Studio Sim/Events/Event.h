#pragma once
#ifndef EVENT_H
#define EVENT_H

// Holds the list of possible events that can occur.
enum class EVENTID
{
	// Mouse Actions
	MousePosition,
  ImGuiMousePosition,
	LeftMouseClick,
	LeftMouseRelease,
	RightMouseClick,
	RightMouseRelease,

	// UI System
	UIKeyInput,
	UIMouseInput,
	RemoveUIItemEvent,
	WorldOrthMatrixEvent,

	// Level System
	SetNextLevelEvent,
	SetCurrentLevelEvent,

	// Game Events
	GamePauseEvent,
	GameUnPauseEvent,
	GameSettingsEvent,
	GameLevelChangeEvent,
	GameEndLevelEvent,

	// Player Movement
	PlayerPosition,
	PlayerUp,
	PlayerLeft,
	PlayerDown,
	PlayerRight,
	PlayerFire,
	TargetPosition,
	
	// Utility
	WindowSizeChangeEvent,
	UpdateSettingsEvent,
	ShowCursorEvent,
	HideCursorEvent,
	QuitGameEvent
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