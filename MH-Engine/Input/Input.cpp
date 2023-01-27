#include "stdafx.h"
#include "Input.h"

void Input::Initialize( RenderWindow& window )
{
	AddToEvent();
    m_renderWindow = window;
	m_fPlayerHealth = new float( 100.0f );

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

		// Update player health
		if ( m_keyboard.KeyIsPressed( 'E' ) )
			*m_fPlayerHealth -= 10.0f;

		if ( m_keyboard.KeyIsPressed( 'Q' ) )
			*m_fPlayerHealth = 100.0f;

		if (m_keyboard.KeyIsPressed('K'))
			EventSystem::Instance()->AddEvent(EVENTID::TomatoKamikaze);

        // Close game
        if ( keycode == VK_ESCAPE )
            EventSystem::Instance()->AddEvent( EVENTID::QuitGameEvent );
	}

	while ( !m_keyboard.CharBufferIsEmpty() )
	{
		unsigned char keycode = m_keyboard.ReadChar();
		m_sKeys += keycode;
		EventSystem::Instance()->AddEvent( EVENTID::KeyInput, &m_sKeys );
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

	EventSystem::Instance()->AddEvent( EVENTID::PlayerHealth, m_fPlayerHealth );
}

void Input::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient( EVENTID::ShowCursorEvent, this );
	EventSystem::Instance()->AddClient( EVENTID::HideCursorEvent, this );
	EventSystem::Instance()->AddClient( EVENTID::ClearCharBuffer, this );
}

void Input::RemoveFromEvent() noexcept
{
	EventSystem::Instance()->RemoveClient( EVENTID::ShowCursorEvent, this );
	EventSystem::Instance()->RemoveClient( EVENTID::HideCursorEvent, this );
	EventSystem::Instance()->RemoveClient( EVENTID::ClearCharBuffer, this );
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
	case EVENTID::ClearCharBuffer:
	{
		m_sKeys.clear();
	}
	break;
	}
}