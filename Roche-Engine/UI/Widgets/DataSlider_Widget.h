#pragma once
#ifndef DATASLIDER_WIDGET_H
#define DATASLIDER_WIDGET_H

#include "MouseData.h"
#include "Transform.h"

class DataSlider_Widget
{
public:
	DataSlider_Widget();
	~DataSlider_Widget();

	void Initialize( ID3D11Device* device, ID3D11DeviceContext* context, ConstantBuffer<Matrices>& mat, int index );
	void Update( const float dt );
	void Draw( ID3D11Device* device, ID3D11DeviceContext* context, XMMATRIX worldOrtho );
	void Resolve( const std::string& barTex, const std::string& sliderTex, MouseData& mData, int index );

	uint32_t GetData() const noexcept { return m_uDataOut; }
	inline std::shared_ptr<Sprite> GetSprite() const noexcept { return m_spriteBar; }
	inline std::shared_ptr<Transform> GetTransform() const noexcept { return m_transformBar; }

private:
	int m_iStart = 50;
	int m_iIdentifier;
	float m_fPx = 0.0f;
	uint32_t m_uDataOut;

	std::string m_barTexture;
	std::shared_ptr<Sprite> m_spriteBar;
	std::shared_ptr<Sprite> m_spriteSlider;

	std::string m_sliderTexture;
	std::shared_ptr<Transform> m_transformBar;
	std::shared_ptr<Transform> m_transformSlider;
};

#endif