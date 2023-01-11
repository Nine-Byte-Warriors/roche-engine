#include "stdafx.h"
#include "Input.h"
#include "Camera.h"

void Input::Initialize( RenderWindow& window )
{
    renderWindow = window;
	pMousePos = new Vector2f();

    // Update keyboard processing
    keyboard.DisableAutoRepeatKeys();
    keyboard.DisableAutoRepeatChars();
}

void Input::Update( float dt )
{
    UpdateMouse( dt );
    UpdateKeyboard( dt );
}

void Input::UpdateMouse( float dt )
{
    // update camera orientation
    while ( !mouse.EventBufferIsEmpty() )
    {
        Mouse::MouseEvent me = mouse.ReadEvent();
        pMousePos->x = me.GetPosX();
        pMousePos->y = me.GetPosY();
        EventSystem::Instance()->AddEvent(EVENTID::PlayerPosition, pMousePos);
        
        if ( mouse.IsRightDown() || !cursorEnabled )
        {
            if ( me.GetType() == Mouse::MouseEvent::EventType::RawMove )
            {
                // Raw mouse movement
            }
        }
    }
}

void Input::UpdateKeyboard( float dt )
{
    // Handle input for single key presses
	while ( !keyboard.KeyBufferIsEmpty() )
	{
		Keyboard::KeyboardEvent kbe = keyboard.ReadKey();
		unsigned char keycode = kbe.GetKeyCode();

		// Set cursor enabled/disabled
		if ( keycode == VK_HOME )
			EnableCursor();
		else if ( keycode == VK_END )
			DisableCursor();

        // Close game
        if ( keycode == VK_ESCAPE )
            EventSystem::Instance()->AddEvent( EVENTID::QuitGameEvent );
	}

    // Handle continuous key presses
    if ( keyboard.KeyIsPressed( 'W' ) )
        EventSystem::Instance()->AddEvent( EVENTID::PlayerUp );

    if ( keyboard.KeyIsPressed( 'A' ) )
        EventSystem::Instance()->AddEvent( EVENTID::PlayerLeft );

    if ( keyboard.KeyIsPressed( 'S' ) )
        EventSystem::Instance()->AddEvent( EVENTID::PlayerDown );

    if ( keyboard.KeyIsPressed( 'D' ) )
        EventSystem::Instance()->AddEvent( EVENTID::PlayerRight );
}