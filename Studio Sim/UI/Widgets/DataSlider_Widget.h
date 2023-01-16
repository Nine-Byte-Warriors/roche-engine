#pragma once
#ifndef DATASLIDER_WIDGET_H
#define DATASLIDER_WIDGET_H

#include "Widget.h"
#include "Transform.h"

class DataSlider_Widget : public Widget
{
public:
	DataSlider_Widget();
	DataSlider_Widget( int start, const std::string& barTex, const std::string& sliderTex, MouseData mData, XMFLOAT2 pos, XMFLOAT2 size );
	~DataSlider_Widget();

	void Initialize( ID3D11Device* device, ID3D11DeviceContext* context, ConstantBuffer<Matrices>& mat );
	void Update( const float dt );
	void Draw( ID3D11Device* device, ID3D11DeviceContext* context, XMMATRIX worldOrtho );
	void Resolve( int& start, const std::string& barTex, const std::string& sliderTex, MouseData mData, XMFLOAT2 pos, XMFLOAT2 size );

	uint32_t GetData() const noexcept { return m_uDataOut; }

private:
	float m_fPx = 0.0f;
	uint32_t m_uDataOut;
	XMFLOAT2 m_vPosition, m_vSize;

	std::string m_barTexture;
	std::shared_ptr<Sprite> m_spriteBar;
	std::shared_ptr<Sprite> m_spriteSlider;

	std::string m_sliderTexture;
	std::shared_ptr<Transform> m_transformBar;
	std::shared_ptr<Transform> m_transformSlider;
};

#endif