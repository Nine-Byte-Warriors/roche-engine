#include "stdafx.h"
#include "Input.h"

void Input::Initialize( RenderWindow& window )
{
	AddToEvent();
    m_renderWindow = window;

    // Update keyboard processing
    m_keyboard.DisableAutoRepeatKeys();
    m_keyboard.DisableAutoRepeatChars();
}

void Input::Update( const float dt )
{
    UpdateMouse( dt );
    UpdateKeyboard( dt );
}

void Input::UpdateMouse( const float dt )
{
    // update camera orientation
    while ( !m_mouse.EventBufferIsEmpty() )
    {
        Mouse::MouseEvent me = m_mouse.ReadEvent();        
        if ( m_mouse.IsRightDown() || !m_bCursorEnabled )
        {
            if ( me.GetType() == Mouse::MouseEvent::EventType::RawMove )
            {
                // Raw mouse movement
            }
        }

		if ( m_mouse.IsLeftDown() || !m_bCursorEnabled )
		{
			if (me.GetType() == Mouse::MouseEvent::EventType::LPress)
			{
				// Left mouse button pressed
			}
			else if (me.GetType() == Mouse::MouseEvent::EventType::LRelease)
			{
				// Left mouse button released
			}
		}
    }
}

void Input::UpdateKeyboard( const float dt )
{
    // Handle input for single key presses
	while ( !m_keyboard.KeyBufferIsEmpty() )
	{
		Keyboard::KeyboardEvent kbe = m_keyboard.ReadKey();
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
    if ( m_keyboard.KeyIsPressed( 'W' ) )
        EventSystem::Instance()->AddEvent( EVENTID::PlayerUp );

    if ( m_keyboard.KeyIsPressed( 'A' ) )
        EventSystem::Instance()->AddEvent( EVENTID::PlayerLeft );

    if ( m_keyboard.KeyIsPressed( 'S' ) )
        EventSystem::Instance()->AddEvent( EVENTID::PlayerDown );

    if ( m_keyboard.KeyIsPressed( 'D' ) )
        EventSystem::Instance()->AddEvent( EVENTID::PlayerRight );
}

void Input::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient( EVENTID::ShowCursorEvent, this );
	EventSystem::Instance()->AddClient( EVENTID::HideCursorEvent, this );
}

void Input::HandleEvent( Event* event )
{
	switch ( event->GetEventID() )
	{
	case EVENTID::ShowCursorEvent:
	{
		EnableCursor();
	}
	break;
	case EVENTID::HideCursorEvent:
	{
		DisableCursor();
	}
	break;
	}
}