#pragma once
#ifndef SPRITE_H
#define SPRITE_H

#include "ConstantBuffer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader_Data.h"
#include "Texture.h"

struct Vertex
{
	XMFLOAT3 Position;
	XMFLOAT2 TexCoord;
};

/// <summary>
/// Create a 2D sprite object.
/// Inherits from GameObject2D.h to allow for modification of position/rotation/scale data in 2D space.
/// </summary>
class Sprite
{
public:
	enum class Type
	{
		Player,
		Carrot,
		Tomato,
		Cauliflower,
		Bean,
		Potato,
		Onion,
		Projectile,
	};

	bool Initialize( ID3D11Device* device, ID3D11DeviceContext* context,
		std::string spritePath, ConstantBuffer<Matrices>& mat,
		float width = 64.0f, float height = 64.0f );
	bool Initialize( ID3D11Device* device, ID3D11DeviceContext* context,
		Colour spriteColour, ConstantBuffer<Matrices>& mat,
		float width = 64.0f, float height = 64.0f );
	bool Initialize( ID3D11Device* device, ID3D11DeviceContext* context,
		Type type, ConstantBuffer<Matrices>& mat,
		float width = 64.0f, float height = 64.0f );
	bool InitializeFromFile(ID3D11Device* device, ID3D11DeviceContext* context,
		std::string sSpritePath, ConstantBuffer<Matrices>& mat, int iCols, int iRows);

	void Draw( XMMATRIX worldMatrix, XMMATRIX orthoMatrix );
	void UpdateBuffers( ID3D11DeviceContext* context );
	void Update( const float dt );

	inline float GetWidth() const noexcept { return m_fWidth; }
	inline void SetWidth( float width ) noexcept { m_fWidth = width; }

	inline float GetHeight() const noexcept { return m_fHeight; }
	inline void SetHeight( float height ) noexcept { m_fHeight = height; }

	inline Vector2f GetWidthHeight() const noexcept { return { m_fWidth, m_fHeight }; };
	inline void SetWidthHeight( Vector2f size ) noexcept { m_fWidth = size.x; m_fHeight = size.y; };
	inline void SetWidthHeight( float width, float height ) noexcept { m_fWidth = width; m_fHeight = height; };

	inline float GetRows() const noexcept { return m_iRows; }
	inline void SetRows( float rows ) noexcept { m_iRows = rows; }

	inline float GetColumns() const noexcept { return m_iColumns; }
	inline void SetColumns( float columns ) noexcept { m_iColumns = columns; }

	inline void SetMaxFrame(int maxFrameX, int maxFrameY) noexcept { m_iMaxFrameX = maxFrameX; m_iMaxFrameY = maxFrameY; }
	inline void SetCurFrameY(int curFrameY) noexcept { m_iCurFrameY = curFrameY; }

	inline void UpdateTex( ID3D11Device* device, std::string tex ) { texture->UpdateTexture( device, tex ); }
	inline void UpdateTex( ID3D11Device* device, Colour tex ) { texture->UpdateTexture( device, tex ); }

	inline void UpdateFrameTime(float frameTime) noexcept { m_fHoldTime = frameTime; }
private:
	bool InitializeInternal( ID3D11Device* device, ID3D11DeviceContext* context,
		ConstantBuffer<Matrices>& mat, float width, float height );

	float m_fWidth;
	float m_fHeight;

	int m_iRows = 1;
	int m_iColumns = 1;

	int m_iCurFrameX = 0;
	int m_iMaxFrameX = 1;

	int m_iCurFrameY = 0;
	int m_iMaxFrameY = 1;

	int m_iCurFrame = 0;
	float m_fHoldTime = 1.0f;
	float m_fCurFrameTime = 0.0f;

	std::unique_ptr<Texture> texture;
	ConstantBuffer<Matrices>* cb_vs_matrix = nullptr;
	ConstantBuffer<Animation> m_cbAnimation;
	ID3D11DeviceContext* context = nullptr;

	VertexBuffer<Vertex> vertices;
	IndexBuffer indices;
	Colour colour;
};

#endif