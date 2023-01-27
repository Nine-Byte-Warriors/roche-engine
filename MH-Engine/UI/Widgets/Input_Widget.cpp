#include "stdafx.h"
#include "Input_Widget.h"
#include "EventSystem.h"

bool Input_Widget::m_bSelected = false;

Input_Widget::Input_Widget()
{
	m_sprite = std::make_shared<Sprite>();
    m_transform = std::make_shared<Transform>( m_sprite );

	std::string keys = "";
	std::string texture = "Resources\\Textures\\Tiles\\empty.png";
	std::vector<std::string> buttonTextures = { texture, texture, texture };
    Resolve( keys, Colors::White, buttonTextures, {} );
}

Input_Widget::Input_Widget( const std::string& texture )
{
	m_sprite = std::make_shared<Sprite>();
    m_transform = std::make_shared<Transform>( m_sprite );

	std::string keys = "";
	std::vector<std::string> buttonTextures = { texture, texture, texture };
	Resolve( keys, Colors::White, buttonTextures, {} );
}

Input_Widget::~Input_Widget() { }

void Input_Widget::Initialize( ID3D11Device* device, ID3D11DeviceContext* context, ConstantBuffer<Matrices>& mat )
{
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
    m_sprite->UpdateTex( device, m_buttonTexture );
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

void Input_Widget::Resolve( std::string& keys, XMVECTORF32 textColour, const std::vector<std::string>& textures, MouseData mData )
{
	m_textColour = textColour;
	m_buttonTexture = textures[0];

    m_transform->SetPosition( m_vPosition.x, m_vPosition.y );
    m_transform->SetScale( m_vSize.x, m_vSize.y );

    m_sprite->SetWidth( m_vSize.x );
    m_sprite->SetHeight( m_vSize.y );

#if !_DEBUG  // not updated for imgui mouse positions
	// Button collison
	if (
		mData.Pos.x >= m_vPosition.x &&
		mData.Pos.x <= ( m_vPosition.x + m_vSize.x ) &&
		mData.Pos.y >= m_vPosition.y &&
		mData.Pos.y <= ( m_vPosition.y + m_vSize.y )
	   )
	{
		m_buttonTexture = textures[1];
		if ( mData.LPress && !mData.Locked )
		{
			EventSystem::Instance()->AddEvent( EVENTID::ReadCharInput, (bool*)true );
			EventSystem::Instance()->AddEvent( EVENTID::ClearCharBuffer, this );
			mData.Locked = true;
			m_bSelected = true;
			m_sCurrText = "";
			keys.clear();
		}
	}
	else
	{
		if ( mData.LPress )
		{
			m_bSelected = false;
			EventSystem::Instance()->AddEvent( EVENTID::ReadCharInput, (bool*)false );
		}
	}
#endif

	if ( m_bSelected )
		m_sCurrText = keys;

	// NOTE: Every frame, this class gets destroyed and everything gets reset
	// m_sCurrText getting reset causes the text to get reset on click outside of button
	// m_bSelected being static means that every button would think that it is active when clicked
}