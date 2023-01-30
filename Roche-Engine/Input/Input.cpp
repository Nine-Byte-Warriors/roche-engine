#include "stdafx.h"
#include "Input.h"

#if _DEBUG
extern bool g_bDebug;
#endif

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
	// Read character input
	if ( m_bReadCharInput )
	{
		while ( !m_keyboard.CharBufferIsEmpty() )
		{
			unsigned char keycode = m_keyboard.ReadChar();
			m_sKeys += keycode;
			EventSystem::Instance()->AddEvent( EVENTID::CharInput, &m_sKeys );
		}
		return;
	}

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

#if _DEBUG
		if ( keycode == VK_F1 )
			g_bDebug = true;
		else if ( keycode == VK_F2 )
			g_bDebug = false;
#endif

		if (m_keyboard.KeyIsPressed('K'))
			EventSystem::Instance()->AddEvent(EVENTID::TomatoKamikaze);

        // Close game
        if ( keycode == VK_ESCAPE )
            EventSystem::Instance()->AddEvent( EVENTID::QuitGameEvent );
	}

    // Handle continuous key presses
#if _DEBUG
    if ( m_keyboard.KeyIsPressed( 'W' ) )
        EventSystem::Instance()->AddEvent( EVENTID::CameraUp );
    if ( m_keyboard.KeyIsPressed( 'A' ) )
        EventSystem::Instance()->AddEvent( EVENTID::CameraLeft );
    if ( m_keyboard.KeyIsPressed( 'S' ) )
        EventSystem::Instance()->AddEvent( EVENTID::CameraDown );
    if ( m_keyboard.KeyIsPressed( 'D' ) )
        EventSystem::Instance()->AddEvent( EVENTID::CameraRight );
#endif
}

void Input::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient( EVENTID::ShowCursorEvent, this );
	EventSystem::Instance()->AddClient( EVENTID::HideCursorEvent, this );
	EventSystem::Instance()->AddClient( EVENTID::ClearCharBuffer, this );
	EventSystem::Instance()->AddClient( EVENTID::ReadCharInput, this );
}

void Input::RemoveFromEvent() noexcept
{
	EventSystem::Instance()->RemoveClient( EVENTID::ShowCursorEvent, this );
	EventSystem::Instance()->RemoveClient( EVENTID::HideCursorEvent, this );
	EventSystem::Instance()->RemoveClient( EVENTID::ClearCharBuffer, this );
	EventSystem::Instance()->RemoveClient( EVENTID::ReadCharInput, this );
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
	case EVENTID::ReadCharInput:
	{
		m_bReadCharInput = static_cast<bool>( event->GetData() );
	}
	break;
	}
}