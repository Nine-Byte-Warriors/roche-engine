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
}

EnergyBar_Widget::~EnergyBar_Widget() { }

void EnergyBar_Widget::Initialize( ID3D11Device* device, ID3D11DeviceContext* context, ConstantBuffer<Matrices>& mat )
{
	m_spriteBack->Initialize( device, context, "", mat );
	m_spriteBar->Initialize( device, context, "", mat );
	m_spriteFront->Initialize( device, context, "", mat );
}

void EnergyBar_Widget::Update( const float dt )
{
	m_transformBack->Update();
	m_transformBar->Update();
	m_transformFront->Update();
}

void EnergyBar_Widget::Draw( ID3D11Device* device, ID3D11DeviceContext* context, XMMATRIX worldOrtho )
{
	// Background
	m_spriteBack->UpdateTex( device, m_textureBack );
	m_spriteBack->UpdateBuffers( context );
	m_spriteBack->Draw( m_transformBack->GetWorldMatrix(), worldOrtho );

	// Moving bar
	//m_spriteBar->UpdateTex( device, m_textureBar ); // use texture
	m_spriteBar->UpdateTex( device, Colour( 255.0f, 100.0f, 100.0f ) ); // use colour
	m_spriteBar->UpdateBuffers( context );
	m_spriteBar->Draw( m_transformBar->GetWorldMatrix(), worldOrtho );

	// Front
	if ( m_textureFront != "" )
	{
		m_spriteFront->UpdateTex( device, m_textureFront );
		m_spriteFront->UpdateBuffers( context );
		m_spriteFront->Draw( m_transformFront->GetWorldMatrix(), worldOrtho );
	}
}

void EnergyBar_Widget::Resolve( const std::vector<std::string>& textures, float fraction )
{
	const float xPadding = 0.8f;
	const float yPadding = 0.5f;
	m_transformBar->SetPosition(
		m_transformBack->GetPosition().x + ( m_spriteBack->GetWidth() * 0.085f ),
		m_transformBack->GetPosition().y + ( m_spriteBack->GetHeight() * 0.25f )
	);
    m_spriteBar->SetWidthHeight(
		m_spriteBack->GetWidth() * m_fCurrentFraction * xPadding,
		m_spriteBack->GetHeight() * yPadding
	);
    m_spriteFront->SetWidth( m_spriteBack->GetWidth() + 13.5f );

	// Update textures
	m_textureBack = textures[0];
	m_textureBar = textures[1];
	m_textureFront = textures[2];

	// Update percentages
	m_iCurrentPercent = fraction;
	m_fCurrentFraction = fraction / 100.0f;
}