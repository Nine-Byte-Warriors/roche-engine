#include "stdafx.h"
#include "Image_Widget.h"

Image_Widget::Image_Widget()
{
	m_sprite = std::make_shared<Sprite>();
	m_transform = std::make_shared<Transform>( m_sprite );
}

Image_Widget::~Image_Widget() { }

void Image_Widget::Initialize( ID3D11Device* device, ID3D11DeviceContext* context, ConstantBuffer<Matrices>& mat )
{
	m_sprite->Initialize( device, context, m_sTextFile, mat );
}

void Image_Widget::Update( const float dt )
{
	m_transform->Update();
}

void Image_Widget::Draw( ID3D11Device* device, ID3D11DeviceContext* context, XMMATRIX worldOrtho )
{
	m_sprite->UpdateTex( device, m_sTextFile );
	m_sprite->UpdateBuffers( context );
	m_sprite->Draw( m_transform->GetWorldMatrix(), worldOrtho );
}

void Image_Widget::Resolve( const std::string& texture )
{
	if ( m_sTextFile != texture )
		m_sTextFile = texture;
}