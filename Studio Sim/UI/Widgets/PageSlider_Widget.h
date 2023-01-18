#pragma once
#ifndef PAGESLIDER_WIDGET_H
#define PAGESLIDER_WIDGET_H

#include "Widget.h"
#include "Transform.h"

class PageSlider_Widget : public Widget
{
public:
	PageSlider_Widget();
    PageSlider_Widget( Colour barCol, Colour sliderCol, MouseData& mData, XMFLOAT2 pos, XMFLOAT2 size );
	~PageSlider_Widget();

	void Initialize( ID3D11Device* device, ID3D11DeviceContext* context, ConstantBuffer<Matrices>& mat );
	void Update( const float dt );
    void Draw( ID3D11Device* device, ID3D11DeviceContext* context, XMMATRIX worldOrtho );
    void Resolve( Colour barCol, Colour sliderCol, MouseData& mData, XMFLOAT2 pos, XMFLOAT2 size );
    
	inline float GetPY() const noexcept{ return m_fPY; }
	inline void SetPY( float py ) noexcept { m_fPY = py; }

	inline float GetPagePos() const noexcept{ return m_fPagePos; }
	inline void SetPageSize( float pageSize ) noexcept { m_fPageSize = pageSize; }

private:
	float m_fPY = 0.0f;
	float m_fPagePos = 0.0f;
	float m_fPageSize = 0.0f;
	XMFLOAT2 m_vPosition, m_vSize;

	Colour m_barColour;
	std::shared_ptr<Sprite> m_spriteBar;
	std::shared_ptr<Sprite> m_spriteSlider;

	Colour m_sliderColour;
	std::shared_ptr<Transform> m_transformBar;
	std::shared_ptr<Transform> m_transformSlider;
};

#endif