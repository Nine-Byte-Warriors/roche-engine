#pragma once
#ifndef UIELEMENT_H
#define UIELEMENT_H

#include "WidgetIncludes.h"
#include "TextRenderer.h"
#include "Listener.h"

class Graphics;

struct TextToDraw
{
	std::string text;
	XMFLOAT2 position;
	XMVECTORF32 color;
};

class UIElement : public Listener
{
public:
	UIElement() {}
	~UIElement() {}

	virtual void Initialize( const Graphics& gfx, ConstantBuffer<Matrices>* mat );
	virtual void Update( const float dt ) = 0;
	virtual void Draw( XMMATRIX worldOrtho, TextRenderer* textRenderer ) = 0;

	virtual void HandleEvent( Event* event ) = 0;
	//virtual void TextLoad() = 0;
	
	inline void SetCB( ConstantBuffer<Matrices>* mat ) noexcept { m_cbMatrices = mat; }
	inline void SetScreenSize( XMFLOAT2 size ) noexcept { m_vScreenSize = size; }

protected:
	std::string ConvertFromUnsignedCharToString( unsigned char input );

	// Inputs
	MouseData m_mouseData;
	unsigned char m_cKey;

	// Graphics
	XMFLOAT2 m_vScreenSize;
	ConstantBuffer<Matrices>* m_cbMatrices;
	
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pContext;
};

#endif