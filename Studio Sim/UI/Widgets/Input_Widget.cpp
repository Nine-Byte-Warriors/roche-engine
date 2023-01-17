#include "stdafx.h"
#include "Input_Widget.h"

Input_Widget::Input_Widget()
{
	m_sprite = std::make_shared<Sprite>();
    m_transform = std::make_shared<Transform>( m_sprite );

	std::string texture = "Resources\\Textures\\Tiles\\empty.png";
    Resolve( 0, Colors::White, texture, {}, { 0.0f, 0.0f }, { 64.0f, 64.0f } );
}

Input_Widget::Input_Widget( const std::string& texture, XMFLOAT2 pos, XMFLOAT2 size )
{
	m_sprite = std::make_shared<Sprite>();
    m_transform = std::make_shared<Transform>( m_sprite );

	Resolve( 0, Colors::White, texture, {}, pos, size );
}

Input_Widget::~Input_Widget() { }

void Input_Widget::Initialize( ID3D11Device* device, ID3D11DeviceContext* context, ConstantBuffer<Matrices>& mat )
{
	m_bSelected = false;
	m_sprite->Initialize( device, context, "", mat, m_vSize.x, m_vSize.y );
    m_transform->SetPositionInit( m_vPosition.x, m_vPosition.y );
	m_transform->SetScaleInit( m_vSize.x, m_vSize.y );
}

void Input_Widget::Update( const float dt )
{
	m_sprite->Update( dt );
	m_transform->Update();
}

void Input_Widget::Draw( ID3D11Device* device, ID3D11DeviceContext* context, XMMATRIX worldOrtho, TextRenderer* textRenderer )
{
	// Button sprite
    m_sprite->UpdateTex( device, m_textureBack );
    m_sprite->UpdateBuffers( context );
    m_sprite->Draw( m_transform->GetWorldMatrix(), worldOrtho );

	XMVECTOR textsize = textRenderer->GetSpriteFont()->MeasureString( m_sCurrText.c_str() );
	XMFLOAT2 textpos =
	{
		m_vPosition.x + ( m_vSize.x / 2.0f ) - ( XMVectorGetX( textsize ) * textRenderer->GetScale().x ) / 2.0f,
		m_vPosition.y + ( m_vSize.y / 2.0f ) - ( XMVectorGetY( textsize ) * textRenderer->GetScale().y ) / 2.0f
	};
	textRenderer->RenderString( m_sCurrText, textpos, m_textColour, false );
}

void Input_Widget::Resolve( unsigned char key, XMVECTORF32 textColour, const std::string& backImg, MouseData mData, XMFLOAT2 pos, XMFLOAT2 size )
{
	m_textureBack = backImg;
	m_textColour = textColour;

	// Update position/scale
    m_vSize = size;
    m_vPosition = pos;

    m_transform->SetPosition( m_vPosition.x, m_vPosition.y );
    m_transform->SetScale( m_vSize.x, m_vSize.y );

    m_sprite->SetWidth( m_vSize.x );
    m_sprite->SetHeight( m_vSize.y );
	
#if !_DEBUG  // not updated for imgui mouse positions
	// Button collison
	if (
		mData.Pos.x >= pos.x &&
		mData.Pos.x <= ( pos.x + size.x ) &&
		mData.Pos.y >= pos.y &&
		mData.Pos.y <= ( pos.y + size.y )
	   )
	{
		if ( mData.LPress )
		{
			m_bSelected = true;
			m_sCurrText = "";
		}
	}
#endif

	// Get text to display
	if ( m_bSelected )
		SetKey( key );
}

void Input_Widget::SetKey( unsigned char key )
{
	switch ( key )
	{
	case VK_RETURN:
		m_sCurrText = "Return";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_SPACE:
		m_sCurrText = "Space";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_SHIFT:
	case VK_LSHIFT:
	case VK_RSHIFT:
		m_sCurrText = "Shift";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_CONTROL:
		m_sCurrText = "Ctrl";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_MENU: // Alt
		m_sCurrText = "alt";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_PAUSE:
		m_sCurrText = "Pause";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_ESCAPE:
		m_sCurrText = "ESC";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_CAPITAL:
		m_sCurrText = "CAPS LOCK";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_BACK:
		m_sCurrText = "BACKSPACE";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_TAB:
		m_sCurrText = "Tab";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_PRIOR: // Page up
		m_sCurrText = "page Up";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_NEXT: // Page down
		m_sCurrText = "page Down";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_END:
		m_sCurrText = "End";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_HOME:
		m_sCurrText = "Home";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_SELECT:
		m_sCurrText = "Select";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_INSERT:
		m_sCurrText = "Ins";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_DELETE:
		m_sCurrText = "Del";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_HELP:
		m_sCurrText = "Help";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_SCROLL:
		m_sCurrText = "Scr Lock";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_CLEAR:
		m_sCurrText = "Clear";
		m_bSelected = false;
		m_cKey = key;
		break;

	// Arrow keys
	case VK_UP:
		m_sCurrText = "Up Arrow";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_DOWN:
		m_sCurrText = "Down Arrow";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_LEFT:
		m_sCurrText = "Left Arrow";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_RIGHT:
		m_sCurrText = "Right Arrow";
		m_bSelected = false;
		m_cKey = key;
		break;

	// Function keys
	case VK_F1:
		m_sCurrText = "F1";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_F2:
		m_sCurrText = "F2";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_F3:
		m_sCurrText = "F3";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_F4:
		m_sCurrText = "F4";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_F5:
		m_sCurrText = "F5";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_F6:
		m_sCurrText = "F6";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_F7:
		m_sCurrText = "F7";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_F8:
		m_sCurrText = "F8";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_F9:
		m_sCurrText = "F9";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_F10:
		m_sCurrText = "F10";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_F11:
		m_sCurrText = "F11";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_F12:
		m_sCurrText = "F12";
		m_bSelected = false;
		m_cKey = key;
		break;
	
	// Numpad
	case VK_NUMLOCK:
		m_sCurrText = "Numlock";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_NUMPAD0:
		m_sCurrText = "Numpad 0";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_NUMPAD1:
		m_sCurrText = "Numpad 1";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_NUMPAD2:
		m_sCurrText = "Numpad 2";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_NUMPAD3:
		m_sCurrText = "Numpad 3";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_NUMPAD4:
		m_sCurrText = "Numpad 4";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_NUMPAD5:
		m_sCurrText = "Numpad 5";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_NUMPAD6:
		m_sCurrText = "Numpad 6";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_NUMPAD7:
		m_sCurrText = "Numpad 7";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_NUMPAD8:
		m_sCurrText = "Numpad 8";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_NUMPAD9:
		m_sCurrText = "Numpad 9";
		m_bSelected = false;
		m_cKey = key;
		break;
	
	// Math keys
	case VK_MULTIPLY:
		m_sCurrText = "*";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_ADD:
		m_sCurrText = "+";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_SEPARATOR:
		m_sCurrText = "";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_SUBTRACT:
		m_sCurrText = "-";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_DECIMAL:
		m_sCurrText = ".";
		m_bSelected = false;
		m_cKey = key;
		break;
	case VK_DIVIDE:
		m_sCurrText = "/";
		m_bSelected = false;
		m_cKey = key;
		break;

	// Number key codes
	case 219:
		m_sCurrText = "[";
		m_bSelected = false;
		m_cKey = key;
		break;
	case 221:
		m_sCurrText = "]";
		m_bSelected = false;
		m_cKey = key;
		break;
	case 222:
		m_sCurrText = "#";
		m_bSelected = false;
		m_cKey = key;
		break;
	case 186:
		m_sCurrText = ";";
		m_bSelected = false;
		m_cKey = key;
		break;
	case 192:
		m_sCurrText = "'";
		m_bSelected = false;
		m_cKey = key;
	case 188:
		m_sCurrText = ",";
		m_bSelected = false;
		m_cKey = key;
		break;
	case 187:
		m_sCurrText = "=";
		m_bSelected = false;
		m_cKey = key;
		break;
	case 223:
		m_sCurrText = "`";
		m_bSelected = false;
		m_cKey = key;
		break;
	case 220:
		m_sCurrText = "\\";
		m_bSelected = false;
		m_cKey = key;
		break;
	case 191:
		m_sCurrText = "/";
		m_bSelected = false;
		m_cKey = key;
		break;
	case 190:
		m_sCurrText = ".";
		m_bSelected = false;
		m_cKey = key;
		break;

	// All other keys
	default:
		if ( key != 0 )
		{
			m_sCurrText = key;
			m_bSelected = false;
			m_cKey = key;
		}
		break;
	}
}