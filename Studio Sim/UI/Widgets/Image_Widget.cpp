#include "stdafx.h"
#include "Image_Widget.h"
#include "Graphics.h"

Image_Widget::Image_Widget()
{
	m_sprite = std::make_shared<Sprite>();
	m_transform = std::make_shared<Transform>( m_sprite );
	Resolve( "Resources\\Textures\\empty.png", { 0.0f, 0.0f }, { 64.0f, 64.0f } );
}

Image_Widget::Image_Widget( const std::string& texture, XMFLOAT2 pos, XMFLOAT2 size )
{
	m_sprite = std::make_shared<Sprite>();
	m_transform = std::make_shared<Transform>( m_sprite );
	Resolve( texture, pos, size );
}

Image_Widget::~Image_Widget() { }

void Image_Widget::Initialize( ID3D11Device* device, ID3D11DeviceContext* context, ConstantBuffer<Matrices>& mat )
{
	m_sprite->Initialize( device, context, m_sTextFile, mat, m_vSize.x, m_vSize.y );
	m_transform->SetPositionInit( m_vPosition.x, m_vPosition.y );
	m_transform->SetScaleInit( m_vSize.x, m_vSize.y );
}

void Image_Widget::Update( const float dt )
{
	m_sprite->Update( dt );
	m_transform->Update();
}

void Image_Widget::Draw( ID3D11Device* device, ID3D11DeviceContext* context, XMMATRIX worldOrtho )
{
	m_sprite->UpdateTex( device, m_sTextFile );
	m_sprite->UpdateBuffers( context );
	m_sprite->Draw( m_transform->GetWorldMatrix(), worldOrtho );
}

void Image_Widget::Resolve( const std::string& texture, XMFLOAT2 pos, XMFLOAT2 size )
{
	m_vSize = size;
	m_vPosition = pos;

	m_transform->SetPosition( m_vPosition.x, m_vPosition.y );
	m_transform->SetScale( m_vSize.x, m_vSize.y );
	
	m_sprite->SetWidth( m_vSize.x );
	m_sprite->SetHeight( m_vSize.y );

	if ( m_sTextFile != texture )
		m_sTextFile = texture;
}