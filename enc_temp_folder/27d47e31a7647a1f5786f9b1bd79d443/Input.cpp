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

	m_keyInputs.emplace( Key::Up, "W" );
	m_keyInputs.emplace( Key::Left, "A" );
	m_keyInputs.emplace( Key::Down, "S" );
	m_keyInputs.emplace( Key::Right, "D" );
	m_keyInputs.emplace( Key::Dash, " " );
	m_keyInputs.emplace( Key::Interact, "E" );
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

		if ( me.GetType() == Mouse::MouseEvent::EventType::WheelUp )
		{
			EventSystem::Instance()->AddEvent(EVENTID::DecrementSeedPacket);
		}
		else if (me.GetType() == Mouse::MouseEvent::EventType::WheelDown)
		{
			EventSystem::Instance()->AddEvent(EVENTID::IncrementSeedPacket);
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
	while (!m_keyboard.KeyBufferIsEmpty())
	{
		Keyboard::KeyboardEvent kbe = m_keyboard.ReadKey();
		unsigned char keycode = kbe.GetKeyCode();

#if _DEBUG
		// Set cursor enabled/disabled
		if ( keycode == VK_HOME )
			EnableCursor();
		else if ( keycode == VK_END )
			DisableCursor();

		if ( keycode == VK_F1 )
			g_bDebug = true;
		else if ( keycode == VK_F2 )
			g_bDebug = false;

		if ( m_keyboard.KeyIsPressed( 'T' ) )
			EventSystem::Instance()->AddEvent( EVENTID::PlantSeed );

		if (m_keyboard.KeyIsPressed('B'))
			EventSystem::Instance()->AddEvent(EVENTID::BuySeed);

		if ( m_keyboard.KeyIsPressed( 'C' ) )
			EventSystem::Instance()->AddEvent( EVENTID::GainCoins );

		if ( m_keyboard.KeyIsPressed( 'L' ) )
			EventSystem::Instance()->AddEvent( EVENTID::PlayerDamage );

		if ( m_keyboard.KeyIsPressed( 'K' ) )
			EventSystem::Instance()->AddEvent( EVENTID::PlayerHeal );
#endif
		if (m_keyboard.KeyIsPressed('H'))
		{
			EventSystem::Instance()->AddEvent(EVENTID::ChangePhase);
		}

        if ( keycode == VK_ESCAPE )
            EventSystem::Instance()->AddEvent( EVENTID::PauseGame );
	}

	// convert keybinds for processing
	std::function<unsigned char(std::string&)> TransformInput = [&](std::string& keyBind) -> unsigned char
	{
		std::transform( keyBind.begin(), keyBind.end(), keyBind.begin(),
			[]( unsigned char c ) { return std::toupper( c ); } );
		return *( reinterpret_cast<unsigned char*>( const_cast<char*>( keyBind.c_str() ) ) );
	};

    // Handle continuous key presses
    if ( m_keyboard.KeyIsPressed( TransformInput( m_keyInputs[Key::Up] ) ) )
        EventSystem::Instance()->AddEvent( EVENTID::MoveUp );

    if ( m_keyboard.KeyIsPressed( TransformInput( m_keyInputs[Key::Left] ) ) )
        EventSystem::Instance()->AddEvent( EVENTID::MoveLeft );

    if ( m_keyboard.KeyIsPressed( TransformInput( m_keyInputs[Key::Down] ) ) )
        EventSystem::Instance()->AddEvent( EVENTID::MoveDown );

    if ( m_keyboard.KeyIsPressed( TransformInput( m_keyInputs[Key::Right] ) ) )
        EventSystem::Instance()->AddEvent( EVENTID::MoveRight );

	if ( m_keyboard.KeyIsPressed( TransformInput( m_keyInputs[Key::Dash] ) ) )
		EventSystem::Instance()->AddEvent( EVENTID::PlayerDash );

	if ( m_keyboard.KeyIsPressed( TransformInput( m_keyInputs[Key::Interact] ) ) )
		EventSystem::Instance()->AddEvent( EVENTID::PlayerInteract );
}

void Input::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient( EVENTID::ShowCursorEvent, this );
	EventSystem::Instance()->AddClient( EVENTID::HideCursorEvent, this );
	EventSystem::Instance()->AddClient( EVENTID::ClearCharBuffer, this );
	EventSystem::Instance()->AddClient( EVENTID::ReadCharInput, this );
	EventSystem::Instance()->AddClient( EVENTID::RemoveHealth, this );
	EventSystem::Instance()->AddClient( EVENTID::KeyInputUpdate_Up, this );
	EventSystem::Instance()->AddClient( EVENTID::KeyInputUpdate_Left, this );
	EventSystem::Instance()->AddClient( EVENTID::KeyInputUpdate_Down, this );
	EventSystem::Instance()->AddClient( EVENTID::KeyInputUpdate_Right, this );
	EventSystem::Instance()->AddClient( EVENTID::KeyInputUpdate_Dash, this );
	EventSystem::Instance()->AddClient( EVENTID::KeyInputUpdate_Interact, this );
}

void Input::RemoveFromEvent() noexcept
{
	EventSystem::Instance()->RemoveClient( EVENTID::ShowCursorEvent, this );
	EventSystem::Instance()->RemoveClient( EVENTID::HideCursorEvent, this );
	EventSystem::Instance()->RemoveClient( EVENTID::ClearCharBuffer, this );
	EventSystem::Instance()->RemoveClient( EVENTID::ReadCharInput, this );
	EventSystem::Instance()->RemoveClient( EVENTID::RemoveHealth, this );
	EventSystem::Instance()->RemoveClient( EVENTID::KeyInputUpdate_Up, this );
	EventSystem::Instance()->RemoveClient( EVENTID::KeyInputUpdate_Left, this );
	EventSystem::Instance()->RemoveClient( EVENTID::KeyInputUpdate_Down, this );
	EventSystem::Instance()->RemoveClient( EVENTID::KeyInputUpdate_Right, this );
	EventSystem::Instance()->RemoveClient( EVENTID::KeyInputUpdate_Dash, this );
	EventSystem::Instance()->RemoveClient( EVENTID::KeyInputUpdate_Interact, this );
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
	case EVENTID::KeyInputUpdate_Up:
	{
		std::string& input = *( static_cast<std::string*>( event->GetData() ) );
		m_keyInputs[Key::Up] = input;
	}
	break;
	case EVENTID::KeyInputUpdate_Left:
	{
		std::string& input = *( static_cast<std::string*>( event->GetData() ) );
		m_keyInputs[Key::Left] = *static_cast<std::string*>( event->GetData() );
	}
	break;
	case EVENTID::KeyInputUpdate_Down:
	{
		std::string& input = *( static_cast<std::string*>( event->GetData() ) );
		m_keyInputs[Key::Down] = *static_cast<std::string*>( event->GetData() );
	}
	break;
	case EVENTID::KeyInputUpdate_Right:
	{
		std::string& input = *( static_cast<std::string*>( event->GetData() ) );
		m_keyInputs[Key::Right] = *static_cast<std::string*>( event->GetData() );
	}
	break;
	case EVENTID::KeyInputUpdate_Dash:
	{
		std::string& input = *( static_cast<std::string*>( event->GetData() ) );
		m_keyInputs[Key::Dash] = *static_cast<std::string*>( event->GetData() );
	}
	break;
	case EVENTID::KeyInputUpdate_Interact:
	{
		std::string& input = *( static_cast<std::string*>( event->GetData() ) );
		m_keyInputs[Key::Interact] = *static_cast<std::string*>( event->GetData() );
	}
	break;
	}
}