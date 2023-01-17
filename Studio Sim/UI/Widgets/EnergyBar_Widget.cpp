#include "stdafx.h"
#include "EnergyBar_Widget.h"

EnergyBar_Widget::EnergyBar_Widget()
{
	m_spriteBack = std::make_shared<Sprite>();
    m_transformBack = std::make_shared<Transform>( m_spriteBack );

	m_spriteBar = std::make_shared<Sprite>();
    m_transformBar = std::make_shared<Transform>( m_spriteBar );

	m_spriteFront = std::make_shared<Sprite>();
    m_transformFront = std::make_shared<Transform>( m_spriteFront );

	std::vector<std::string> textures =
	{
		"Resources\\Textures\\UI\\Button\\ButtonIdle.png",
		"Resources\\Textures\\UI\\Button\\ButtonIdle.png",
		"Resources\\Textures\\UI\\Button\\ButtonIdle.png"
	};
	Resolve( textures, 50.0f, { 0.0f, 0.0f }, { 64.0f, 64.0f } );
}

EnergyBar_Widget::EnergyBar_Widget( const std::vector<std::string>& textures, float fraction, XMFLOAT2 pos, XMFLOAT2 size )
{
	m_spriteBack = std::make_shared<Sprite>();
    m_transformBack = std::make_shared<Transform>( m_spriteBack );

	m_spriteBar = std::make_shared<Sprite>();
    m_transformBar = std::make_shared<Transform>( m_spriteBar );

	m_spriteFront = std::make_shared<Sprite>();
    m_transformFront = std::make_shared<Transform>( m_spriteFront );

	Resolve( textures, fraction, pos, size );
}

EnergyBar_Widget::~EnergyBar_Widget() { }

void EnergyBar_Widget::Initialize( ID3D11Device* device, ID3D11DeviceContext* context, ConstantBuffer<Matrices>& mat )
{
	m_spriteBack->Initialize( device, context, "", mat, m_vSize.x, m_vSize.y );
    m_transformBack->SetPositionInit( m_vPosition.x, m_vPosition.y );
	m_transformBack->SetScaleInit( m_vSize.x, m_vSize.y );

	m_spriteBar->Initialize( device, context, "", mat, m_vSize.x, m_vSize.y );
    m_transformBar->SetPositionInit( m_vPosition.x, m_vPosition.y );
	m_transformBar->SetScaleInit( m_vSize.x, m_vSize.y );

	m_spriteFront->Initialize( device, context, "", mat, m_vSize.x, m_vSize.y );
    m_transformFront->SetPositionInit( m_vPosition.x, m_vPosition.y );
	m_transformFront->SetScaleInit( m_vSize.x, m_vSize.y );
}

void EnergyBar_Widget::Update( const float dt )
{
	m_spriteBack->Update( dt );
	m_transformBack->Update();

	m_spriteBar->Update( dt );
	m_transformBar->Update();

	m_spriteFront->Update( dt );
	m_transformFront->Update();
}

void EnergyBar_Widget::Draw( ID3D11Device* device, ID3D11DeviceContext* context, XMMATRIX worldOrtho )
{
	// Background
	m_transformBack->SetPosition( m_vPosition.x, m_vPosition.y );
	m_transformBack->SetScale( m_vSize.x, m_vSize.y );

	m_spriteBack->SetWidth( m_vSize.x );
	m_spriteBack->SetHeight( m_vSize.y );

	m_spriteBack->UpdateTex( device, m_textureBack );
	m_spriteBack->UpdateBuffers( context );
	m_spriteBack->Draw( m_transformBack->GetWorldMatrix(), worldOrtho );

	// Moving bar
	const float xPadding = 0.8f;
	const float yPadding = 0.5f;
	m_transformBar->SetPosition(
		m_transformBack->GetPosition().x + ( m_spriteBack->GetWidth() * 0.085f ),
		m_transformBack->GetPosition().y + ( m_spriteBack->GetHeight() * 0.25f )
	);
	m_transformBar->SetScale( m_vSize.x * m_fCurrentFraction * xPadding, m_vSize.y * yPadding );

	m_spriteBar->SetWidth( m_vSize.x * m_fCurrentFraction * xPadding );
	m_spriteBar->SetHeight( m_vSize.y * yPadding );

	//m_spriteBar->UpdateTex( device, m_textureBar ); // use texture
	m_spriteBar->UpdateTex( device, Colour( 255.0f, 100.0f, 100.0f ) ); // use colour
	m_spriteBar->UpdateBuffers( context );
	m_spriteBar->Draw( m_transformBar->GetWorldMatrix(), worldOrtho );

	// Front
	if ( m_textureFront != "" )
	{
		m_transformFront->SetPosition( m_vPosition.x, m_vPosition.y );
		m_transformFront->SetScale( m_vSize.x + 13.5f, m_vSize.y );

		m_spriteFront->SetWidth( m_vSize.x + 13.5f );
		m_spriteFront->SetHeight( m_vSize.y );

		m_spriteFront->UpdateTex( device, m_textureFront );
		m_spriteFront->UpdateBuffers( context );
		m_spriteFront->Draw( m_transformFront->GetWorldMatrix(), worldOrtho );
	}
}

void EnergyBar_Widget::Resolve( const std::vector<std::string>& textures, float fraction, XMFLOAT2 pos, XMFLOAT2 size )
{
	// Update position/scale
    m_vSize = size;
    m_vPosition = pos;

    m_transformBack->SetPosition( m_vPosition.x, m_vPosition.y );
    m_transformBack->SetScale( m_vSize.x, m_vSize.y );
    m_spriteBack->SetWidth( m_vSize.x );
    m_spriteBack->SetHeight( m_vSize.y );

	const float xPadding = 0.8f;
	const float yPadding = 0.5f;
	m_transformBar->SetPosition(
		m_transformBack->GetPosition().x + ( m_spriteBack->GetWidth() * 0.085f ),
		m_transformBack->GetPosition().y + ( m_spriteBack->GetHeight() * 0.25f )
	);
    m_transformBar->SetScale( m_vSize.x * m_fCurrentFraction * xPadding, m_vSize.y * yPadding );
    m_spriteBar->SetWidth( m_vSize.x * m_fCurrentFraction * xPadding );
    m_spriteBar->SetHeight( m_vSize.y * yPadding );

	m_transformFront->SetPosition( m_vPosition.x, m_vPosition.y );
    m_transformFront->SetScale( m_vSize.x + 13.5f, m_vSize.y );
    m_spriteFront->SetWidth( m_vSize.x + 13.5f );
    m_spriteFront->SetHeight( m_vSize.y );

	// Update textures
	m_textureBack = textures[0];
	m_textureBar = textures[1];
	m_textureFront = textures[2];

	// Update percentages
	m_iCurrentPercent = fraction;
	m_fCurrentFraction = fraction / 100.0f;
}