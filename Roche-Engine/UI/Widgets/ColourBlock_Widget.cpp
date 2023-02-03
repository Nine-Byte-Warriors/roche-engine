#include "stdafx.h"
#include "ColourBlock_Widget.h"

ColourBlock_Widget::ColourBlock_Widget( XMFLOAT2 pos, XMFLOAT2 size )
{
	m_sprite = std::make_shared<Sprite>();
    m_sprite->SetWidthHeight( size.x, size.y );

    m_transform = std::make_shared<Transform>();
    m_transform->SetPosition( { pos.x, pos.y } );
	Resolve( { 210, 210, 150 } );
}

ColourBlock_Widget::ColourBlock_Widget( Colour colour, XMFLOAT2 pos, XMFLOAT2 size )
{
	m_sprite = std::make_shared<Sprite>();
    m_sprite->SetWidthHeight( size.x, size.y );

    m_transform = std::make_shared<Transform>();
    m_transform->SetPosition( { pos.x, pos.y } );
	Resolve( colour );
}

ColourBlock_Widget::~ColourBlock_Widget() { }

void ColourBlock_Widget::Initialize( ID3D11Device* device, ID3D11DeviceContext* context, ConstantBuffer<Matrices>& mat )
{
	m_sprite->Initialize( device, context, "", mat );
}

void ColourBlock_Widget::Update( const float dt )
{
	m_transform->UpdateSprite( m_sprite );
	m_transform->Update();
}

void ColourBlock_Widget::Draw( ID3D11Device* device, ID3D11DeviceContext* context, XMMATRIX worldOrtho )
{
	m_sprite->UpdateTex( device, m_colour );
	m_sprite->UpdateBuffers( context );
	m_sprite->Draw( m_transform->GetWorldMatrix(), worldOrtho );
}

void ColourBlock_Widget::Resolve( Colour colour )
{
	m_colour = colour;
}