#include "stdafx.h"
#include "ColourBlock_Widget.h"
#include "Graphics.h"

ColourBlock_Widget::ColourBlock_Widget()
{
	m_sprite = std::make_shared<Sprite>();
	m_transform = std::make_shared<Transform>( m_sprite );
	Resolve( { 210, 210, 150 }, { 1.0f, 1.0f }, { 0.0f, 0.0f } );
}

ColourBlock_Widget::ColourBlock_Widget( Colour colour, XMFLOAT2 pos, XMFLOAT2 size )
{
	m_sprite = std::make_shared<Sprite>();
	m_transform = std::make_shared<Transform>( m_sprite );
	Resolve( colour, size, pos );
}

ColourBlock_Widget::~ColourBlock_Widget() { }

void ColourBlock_Widget::Initialize( const Graphics& gfx, ConstantBuffer<Matrices>& mat )
{
	m_sprite->Initialize( gfx.GetDevice(), gfx.GetContext(), m_colour, mat, m_vSize.x, m_vSize.y );
	m_transform->SetPositionInit( m_vPosition.x, m_vPosition.y );
	m_transform->SetScaleInit( m_vSize.x, m_vSize.y );
}

void ColourBlock_Widget::Update( const float dt )
{
	m_sprite->Update( dt );
	m_transform->Update();
}

void ColourBlock_Widget::Draw( const Graphics& gfx, XMMATRIX worldOrtho )
{
	m_sprite->UpdateTex( gfx.GetDevice(), m_colour );
	m_sprite->UpdateBuffers( gfx.GetContext() );
	m_sprite->Draw( m_transform->GetWorldMatrix(), worldOrtho );
}

void ColourBlock_Widget::Resolve( Colour colour, XMFLOAT2 pos, XMFLOAT2 size )
{
	m_vSize = size;
	m_colour = colour;
	m_vPosition = pos;

	m_transform->SetPosition( m_vPosition.x, m_vPosition.y );
	m_transform->SetScale( m_vSize.x, m_vSize.y );
	
	m_sprite->SetWidth( m_vSize.x );
	m_sprite->SetHeight( m_vSize.y );
}