#pragma once
#ifndef CUBE_H
#define CUBE_H

class Camera;
#include "Resource.h"
#include "DDSTextureLoader.h"

#include "structures.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"

struct Vertex
{
	XMFLOAT3 Position;
	XMFLOAT2 TexCoord;
};

class Cube
{
public:
	bool InitializeMesh( ID3D11Device* pDevice, ID3D11DeviceContext* pContext );
	void Update( float dt );
	void UpdateBuffers( ConstantBuffer<Matrices>& cb_vs_matrices, const Camera& pCamera );
	void Draw( ID3D11DeviceContext* pContext );
	void SpawnControlWindow();

	inline XMFLOAT4X4* GetTransform() noexcept { return &m_World; }
	
	inline void SetPosition( XMFLOAT3 position ) { m_position = position; UpdateMatrix(); }
	inline XMFLOAT3 GetPosition() const noexcept { return m_position; }

	inline void SetRotation( XMFLOAT3 rotation ) { m_rotation = rotation; UpdateMatrix(); }
	inline XMFLOAT3 GetRotation() const noexcept { return m_rotation; }
	
	inline void SetScale( XMFLOAT3 scale ) { m_scale = scale; UpdateMatrix(); };
	inline XMFLOAT3 GetScale() const noexcept { return m_scale; }

private:
	void UpdateMatrix();

	XMFLOAT4X4 m_World;
	XMFLOAT3 m_position;
	XMFLOAT3 m_rotation;
	XMFLOAT3 m_scale;
	
	bool m_bResetSpin = false;
	bool m_bEnableSpin = false;
	bool m_bReverseSpin = false;

	IndexBuffer m_indexBuffer;
	VertexBuffer<Vertex> m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTextureDiffuse;
};

#endif