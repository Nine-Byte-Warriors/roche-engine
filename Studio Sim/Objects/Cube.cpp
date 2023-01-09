#include "stdafx.h"
#include "Cube.h"
#include "Camera.h"
#include <imgui/imgui.h>

Vertex vertices[] =
{
	{ { -1.0f,  1.0f, -1.0f }, { 1.0f, 0.0f } },
	{ {  1.0f,  1.0f, -1.0f }, { 0.0f, 0.0f } },
	{ {  1.0f,  1.0f,  1.0f }, { 0.0f, 1.0f } },
	{ { -1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f } },
							   
	{ { -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f } },
	{ {  1.0f, -1.0f, -1.0f }, { 1.0f, 0.0f } },
	{ {  1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f } },
	{ { -1.0f, -1.0f,  1.0f }, { 0.0f, 1.0f } },
							   
	{ { -1.0f, -1.0f,  1.0f }, { 0.0f, 1.0f } },
	{ { -1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f } },
	{ { -1.0f,  1.0f, -1.0f }, { 1.0f, 0.0f } },
	{ { -1.0f,  1.0f,  1.0f }, { 0.0f, 0.0f } },
							   
	{ {  1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f } },
	{ {  1.0f, -1.0f, -1.0f }, { 0.0f, 1.0f } },
	{ {  1.0f,  1.0f, -1.0f }, { 0.0f, 0.0f } },
	{ {  1.0f,  1.0f,  1.0f }, { 1.0f, 0.0f } },
							   
	{ { -1.0f, -1.0f, -1.0f }, { 0.0f, 1.0f } },
	{ {  1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f } },
	{ {  1.0f,  1.0f, -1.0f }, { 1.0f, 0.0f } },
	{ { -1.0f,  1.0f, -1.0f }, { 0.0f, 0.0f } },
							   
	{ { -1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f } },
	{ {  1.0f, -1.0f,  1.0f }, { 0.0f, 1.0f } },
	{ {  1.0f,  1.0f,  1.0f }, { 0.0f, 0.0f } },
	{ { -1.0f,  1.0f,  1.0f }, { 1.0f, 0.0f } },
};

WORD indices[] =
{
	3,1,0,
	2,1,3,

	6,4,5,
	7,4,6,

	11,9,8,
	10,9,11,

	14,12,13,
	15,12,14,

	19,17,16,
	18,17,19,

	22,20,21,
	23,20,22
};

bool Cube::InitializeMesh( ID3D11Device* pDevice, ID3D11DeviceContext* pContext )
{
	try
	{
		// Set position to world origin
		XMStoreFloat4x4( &m_World, XMMatrixIdentity() );
		m_position = { 0.0f, 0.0f, 10.0f };
		m_rotation = { 0.0f, 0.0f, 0.0f };
		m_scale = { 1.0f, 1.0f, 1.0f };

		// Create vertex buffer
		HRESULT hr = m_vertexBuffer.Initialize( pDevice, vertices, ARRAYSIZE( vertices ) );
        COM_ERROR_IF_FAILED( hr, "Failed to create cube vertex buffer!" );
		
		// Create index buffer
        hr = m_indexBuffer.Initialize( pDevice, indices, ARRAYSIZE( indices ) );
        COM_ERROR_IF_FAILED( hr, "Failed to create cube index buffer!" );

		// Load and setup textures
		hr = DirectX::CreateDDSTextureFromFile( pDevice, L"Resources\\Textures\\bricks_TEX.dds", nullptr, m_pTextureDiffuse.GetAddressOf() );
		COM_ERROR_IF_FAILED( hr, "Failed to create 'diffuse' texture!" );
	}
	catch ( COMException& exception )
	{
		ErrorLogger::Log( exception );
		return false;
	}

	return true;
}

void Cube::Update( float dt )
{
	if ( m_bEnableSpin )
	{
		if ( m_bReverseSpin )
			m_rotation.y -= dt;
		else
			m_rotation.y += dt;
	}

	UpdateMatrix();
}

void Cube::UpdateMatrix()
{
	XMMATRIX world = XMMatrixScaling( m_scale.x, m_scale.y, m_scale.z ) *
		XMMatrixRotationRollPitchYaw( m_rotation.x, m_rotation.y, m_rotation.z ) *
		XMMatrixTranslation( m_position.x, m_position.y, m_position.z );
	XMStoreFloat4x4( &m_World, world );
}

void Cube::UpdateBuffers( ConstantBuffer<Matrices>& cb_vs_matrices, const Camera& pCamera )
{
	// Get the game object world transform
    XMMATRIX mGO = XMLoadFloat4x4( &m_World );
	cb_vs_matrices.data.mWorld = XMMatrixTranspose( mGO );
    
    // Store the view / projection in a constant buffer for the vertex shader to use
	cb_vs_matrices.data.mView = XMMatrixTranspose( pCamera.GetViewMatrix() );
	cb_vs_matrices.data.mProjection = XMMatrixTranspose( pCamera.GetProjectionMatrix() );
	if ( !cb_vs_matrices.ApplyChanges() ) return;
}

void Cube::Draw( ID3D11DeviceContext* pContext )
{
	UINT offset = 0u;
	pContext->IASetVertexBuffers( 0u, 1u, m_vertexBuffer.GetAddressOf(), m_vertexBuffer.StridePtr(), &offset );
	pContext->IASetIndexBuffer( m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u );
	pContext->PSSetShaderResources( 0u, 1u, m_pTextureDiffuse.GetAddressOf() );
	pContext->DrawIndexed( m_indexBuffer.IndexCount(), 0u, 0u );
}

void Cube::SpawnControlWindow()
{
	if ( ImGui::Begin( "Cube Data", FALSE, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove ) )
	{
		ImGui::Checkbox( "Rotate Cube?", &m_bEnableSpin );
		ImGui::Checkbox( "Reverse?", &m_bReverseSpin );
		if ( ImGui::Checkbox( "Reset?", &m_bResetSpin ) )
		{
			m_rotation.y = 0.0f;
			m_bResetSpin = false;
			m_bEnableSpin = false;
			m_bReverseSpin = false;
		}
	}
	ImGui::End();
}