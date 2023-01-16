#include "stdafx.h"
#include "Sprite.h"
#include <dxtk/WICTextureLoader.h>

bool Sprite::Initialize( ID3D11Device* device, ID3D11DeviceContext* context,
	std::string spritePath, ConstantBuffer<Matrices>& mat, float width, float height )
{
	texture = std::make_unique<Texture>( device, spritePath, aiTextureType_DIFFUSE );
	return InitializeInternal( device, context, mat, width, height );
}

bool Sprite::Initialize( ID3D11Device* device, ID3D11DeviceContext* context,
	Colour spriteColour, ConstantBuffer<Matrices>& mat, float width, float height )
{
	texture = std::make_unique<Texture>( device, spriteColour, aiTextureType_DIFFUSE );
	return InitializeInternal( device, context, mat, width, height );
}

bool Sprite::Initialize( ID3D11Device* device, ID3D11DeviceContext* context,
	Type type, ConstantBuffer<Matrices>& mat, float width, float height )
{
	std::string spritePath;
	switch ( type )
	{
	case Type::Player:		spritePath = "Resources\\Textures\\carrot_ss.png";		 m_iMaxFrameX = 2; m_iMaxFrameY = 2; break;
	case Type::Carrot:		spritePath = "Resources\\Textures\\carrot_ss.png";		 m_iMaxFrameX = 2; m_iMaxFrameY = 2; break;
	case Type::Tomato:		spritePath = "Resources\\Textures\\tomato_ss.png";		 m_iMaxFrameX = 2; m_iMaxFrameY = 2; break;
	case Type::Cauliflower: spritePath = "Resources\\Textures\\cauliflower_ss.png";  m_iMaxFrameX = 1; m_iMaxFrameY = 1; break;
	case Type::Bean:		spritePath = "Resources\\Textures\\bean_ss.png";		 m_iMaxFrameX = 1; m_iMaxFrameY = 2; break;
	//case Type::Potato:	spritePath = "Resources\\Textures\\potato_ss.png";		 m_iMaxFrameX = 2; m_iMaxFrameY = 2; break;
	case Type::Onion:		spritePath = "Resources\\Textures\\onion_ss.png";		 m_iMaxFrameX = 2; m_iMaxFrameY = 2; break;
	case Type::Projectile:	spritePath = "Resources\\Textures\\Base_Projectile.png"; m_iMaxFrameX = 1; m_iMaxFrameY = 1; break;
	default: break;
	}

	return Initialize( device, context, spritePath, mat );
}

bool Sprite::InitializeInternal( ID3D11Device* device, ID3D11DeviceContext* context,
	ConstantBuffer<Matrices>& mat, float width, float height )
{
	m_fWidth = width;
	m_fHeight = height;

	this->context = context;
	if ( context == nullptr ) return false;

	cb_vs_matrix = &mat;
	if ( cb_vs_matrix == nullptr ) return false;

	std::vector<Vertex> vertexData =
	{
		{ { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f } },
		{ {  0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f } },
		{ { -0.5f,  0.5f, 0.0f }, { 0.0f, 1.0f } },
		{ {  0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f } },
	};

	std::vector<WORD> indexData =
	{
		0, 1, 2,
		2, 1, 3
	};

	try
	{
		HRESULT hr = vertices.Initialize( device, vertexData.data(), static_cast<UINT>( vertexData.size() ) );
		COM_ERROR_IF_FAILED( hr, "Failed to initialize vertices for sprite!" );

		hr = indices.Initialize( device, indexData.data(), static_cast<UINT>( indexData.size() ) );
		COM_ERROR_IF_FAILED( hr, "Failed to initialize indices for sprite!" );

		hr = m_cbAnimation.Initialize( device, context );
		COM_ERROR_IF_FAILED( hr, "Failed to create 'Animation' constant buffer!" );
	}
	catch ( COMException& exception )
	{
		ErrorLogger::Log( exception );
        return false;
	}
}

void Sprite::Draw( XMMATRIX worldMatrix, XMMATRIX orthoMatrix )
{
	XMMATRIX wvpMatrix = worldMatrix * orthoMatrix;
	context->VSSetConstantBuffers( 0, 1, cb_vs_matrix->GetAddressOf() );
	cb_vs_matrix->data.wvpMatrix = wvpMatrix;
	if ( !cb_vs_matrix->ApplyChanges() ) return;
	context->PSSetShaderResources( 0, 1, texture->GetTextureResourceViewAddress() );

	const UINT offsets = 0;
	context->IASetVertexBuffers( 0, 1, vertices.GetAddressOf(), vertices.StridePtr(), &offsets );
	context->IASetIndexBuffer( indices.Get(), DXGI_FORMAT_R16_UINT, 0 );
	context->DrawIndexed( indices.IndexCount(), 0, 0 );
}

void Sprite::UpdateBuffers( ID3D11DeviceContext* context )
{
	m_cbAnimation.data.Width = m_fWidth;
	m_cbAnimation.data.Height = m_fHeight;
	m_cbAnimation.data.Rows = m_iMaxFrameX;
	m_cbAnimation.data.Columns = m_iMaxFrameY;
	m_cbAnimation.data.FrameX = m_iCurFrameX;
	m_cbAnimation.data.FrameY = m_iCurFrameY;
	if ( !m_cbAnimation.ApplyChanges() ) return;
	context->VSSetConstantBuffers( 1u, 1u, m_cbAnimation.GetAddressOf() );
}

void Sprite::Update( const float dt )
{
	m_fCurFrameTime += dt;
	while ( m_fCurFrameTime >= m_fHoldTime )
	{
		m_iCurFrameX++;
		if ( m_iCurFrameX >= m_iMaxFrameX )
			m_iCurFrameX = 0;
		m_fCurFrameTime -= m_fHoldTime;
	}
}