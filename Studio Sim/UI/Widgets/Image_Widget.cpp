#include "stdafx.h"
#include "Image_Widget.h"
#include "Graphics.h"

Image_Widget::Image_Widget()
{
	m_sTextFile = "";
	m_vSize = { 1.0f, 1.0f };
	m_vPosition = { 0.0f, 0.0f };

	m_sprite = std::make_shared<Sprite>();
	m_transform = std::make_shared<Transform>( m_sprite );
}

Image_Widget::Image_Widget( const std::string& texture, XMFLOAT2 pos, XMFLOAT2 size )
{
	m_sTextFile = texture;
	m_vSize = { 1.0f, 1.0f };
	m_vPosition = { 0.0f, 0.0f };

	m_sprite = std::make_shared<Sprite>();
	m_transform = std::make_shared<Transform>( m_sprite );
}

Image_Widget::~Image_Widget() { }

void Image_Widget::Initialize( const Graphics& gfx, ConstantBuffer<Matrices>& mat )
{
	m_sprite->Initialize( gfx.GetDevice(), gfx.GetContext(), m_sTextFile, mat, m_vSize.x, m_vSize.y );
	m_transform->SetPositionInit( m_vPosition.x, m_vPosition.y );
	m_transform->SetScaleInit( m_vSize.x, m_vSize.y );
}

void Image_Widget::Update( const float dt )
{
	m_sprite->Update( dt );
	m_transform->Update();
}

void Image_Widget::Draw( const Graphics& gfx, XMMATRIX worldOrtho )
{
	m_sprite->UpdateTex( gfx.GetDevice(), m_sTextFile );
	m_sprite->UpdateBuffers( gfx.GetContext() );
	m_sprite->Draw( m_transform->GetWorldMatrix(), worldOrtho );
}

void Image_Widget::Resolve( const std::string& texture, XMFLOAT2 size, XMFLOAT2 pos )
{
	std::string textFile = "Resources\\Textures\\";
	textFile.append( texture );
	m_vPosition = pos;
	m_vSize = size;

	m_transform->SetPosition( m_vPosition.x, m_vPosition.y );
	m_transform->SetScale( m_vSize.x, m_vSize.y );
	
	m_sprite->SetWidth( m_vSize.x );
	m_sprite->SetHeight( m_vSize.y );

	if ( m_sTextFile != texture )
		m_sTextFile = texture;
}