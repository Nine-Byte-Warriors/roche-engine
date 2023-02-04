#pragma once
#ifndef PAGESLIDER_WIDGET_H
#define PAGESLIDER_WIDGET_H

#include "MouseData.h"
#include "Transform.h"

class PageSlider_Widget
{
public:
	PageSlider_Widget();
	~PageSlider_Widget();

	void Initialize( ID3D11Device* device, ID3D11DeviceContext* context, ConstantBuffer<Matrices>& mat, float pageSizeX, float pageSizeY );
	void Update( const float dt );
    void Draw( ID3D11Device* device, ID3D11DeviceContext* context, XMMATRIX worldOrtho );
    void Resolve( Colour barCol, Colour sliderCol, MouseData& mData );

	inline float GetPY() const noexcept{ return m_fPY; }
	inline void SetPY( float py ) noexcept { m_fPY = py; }

	inline float GetPagePos() const noexcept{ return m_fPagePos; }
	inline void SetPageSizeX( float pageSizeX ) noexcept { m_fPageSizeX = pageSizeX; }
	inline void SetPageSizeY( float pageSizeY ) noexcept { m_fPageSizeY = pageSizeY; }

	inline std::shared_ptr<Sprite> GetSprite() const noexcept { return m_spriteBar; }
	inline std::shared_ptr<Transform> GetTransform() const noexcept { return m_transformBar; }

private:
	float m_fPY = 0.0f;
	float m_fPagePos = 0.0f;
	float m_fPageSizeX = 0.0f;
	float m_fPageSizeY = 0.0f;

	Colour m_barColour;
	std::shared_ptr<Sprite> m_spriteBar;
	std::shared_ptr<Sprite> m_spriteSlider;

	Colour m_sliderColour;
	std::shared_ptr<Transform> m_transformBar;
	std::shared_ptr<Transform> m_transformSlider;
};

#endif