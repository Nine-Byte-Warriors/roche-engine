#pragma once
#ifndef WIDGET_H
#define WIDGET_H

#include "Button_Widget.h"
#include "ColourBlock_Widget.h"
#include "DataSlider_Widget.h"
#include "DropDown_Widget.h"
#include "EnergyBar_Widget.h"
#include "Image_Widget.h"
#include "Input_Widget.h"

class Widget
{
public:
	Widget()
		: m_bHidden( false ), m_iZIndex( 0 ), m_sName( "Empty" ), m_sType( "Empty" ), m_sAction( "" ), m_vPosition( { 0.0f, 0.0f } ), m_vSize( { 0.0f, 0.0f } ),
		m_pButton( std::make_shared<Button_Widget>( m_vPosition, m_vSize ) ),
		m_pColourBlock( std::make_shared<ColourBlock_Widget>( m_vPosition, m_vSize ) ),
		m_pDataSlider( std::make_shared<DataSlider_Widget>( m_vPosition, m_vSize ) ),
		m_pDropDown( std::make_shared<DropDown_Widget>( m_vPosition, m_vSize ) ),
		m_pEnergyBar( std::make_shared<EnergyBar_Widget>( m_vPosition, m_vSize ) ),
		m_pImage( std::make_shared<Image_Widget>( m_vPosition, m_vSize ) ),
		m_pInput( std::make_shared<Input_Widget>( m_vPosition, m_vSize ) )
	{}
	Widget( bool hide, int zIdx, std::string name, std::string type, std::string action, XMFLOAT2 pos, XMFLOAT2 scale )
		: m_bHidden( hide ), m_iZIndex( zIdx ), m_sName( name ), m_sType( type ), m_sAction( action ), m_vPosition( pos ), m_vSize( scale ),
		m_pButton( std::make_shared<Button_Widget>( m_vPosition, m_vSize ) ),
		m_pColourBlock( std::make_shared<ColourBlock_Widget>( m_vPosition, m_vSize ) ),
		m_pDataSlider( std::make_shared<DataSlider_Widget>( m_vPosition, m_vSize ) ),
		m_pDropDown( std::make_shared<DropDown_Widget>( m_vPosition, m_vSize ) ),
		m_pEnergyBar( std::make_shared<EnergyBar_Widget>( m_vPosition, m_vSize ) ),
		m_pImage( std::make_shared<Image_Widget>( m_vPosition, m_vSize ) ),
		m_pInput( std::make_shared<Input_Widget>( m_vPosition, m_vSize ) )
	{}
	~Widget()
	{}

	virtual void Resolve() {}
	virtual void Draw( ID3D11Device* device, ID3D11DeviceContext* context, XMMATRIX worldOrtho ) {}

	inline int GetZIndex() const noexcept { return m_iZIndex; }
	inline XMFLOAT2 GetSize() const noexcept { return m_vSize; }
	inline std::string GetName() const noexcept { return m_sName; }
	inline std::string GetType() const noexcept { return m_sType; }
	inline std::string GetAction() const noexcept { return m_sAction; }
	inline XMFLOAT2 GetPosition() const noexcept { return m_vPosition; }
	inline bool GetIsHidden() const noexcept { return m_bHidden; }

	inline void SetZIndex( const int& idx ) noexcept { m_iZIndex = idx; }
	inline void SetName( const std::string& name ) noexcept { m_sName = name; }
	inline void SetType( const std::string& type ) noexcept { m_sType = type; }
	inline void SetAction( const std::string& action ) noexcept { m_sAction = action; }
	inline void SetIsHidden( bool hide ) noexcept { m_bHidden = hide; }

	inline void SetSize( const XMFLOAT2& size ) noexcept
	{
		m_vSize = size;
		if ( m_sType == "Button" )
			m_pButton->GetSprite()->SetWidthHeight( m_vSize.x, m_vSize.y );
		else if ( m_sType == "Colour Block" )
			m_pColourBlock->GetSprite()->SetWidthHeight( m_vSize.x, m_vSize.y );
		else if ( m_sType == "Data Slider" )
			m_pDataSlider->GetSprite()->SetWidthHeight( m_vSize.x, m_vSize.y );
		else if ( m_sType == "Drop Down" )
			m_pDropDown->GetSprite()->SetWidthHeight( m_vSize.x, m_vSize.y );
		else if ( m_sType == "Energy Bar" )
			m_pEnergyBar->GetSprite()->SetWidthHeight( m_vSize.x, m_vSize.y );
		else if ( m_sType == "Image" )
			m_pImage->GetSprite()->SetWidthHeight( m_vSize.x, m_vSize.y );
		else if ( m_sType == "Input" )
			m_pInput->GetSprite()->SetWidthHeight( m_vSize.x, m_vSize.y );
	}

	inline void SetPosition( const XMFLOAT2& pos ) noexcept
	{
		m_vPosition = pos;
		if ( m_sType == "Button" )
			m_pButton->GetTransform()->SetPosition( { m_vPosition.x, m_vPosition.y } );
		else if ( m_sType == "Colour Block" )
			m_pColourBlock->GetTransform()->SetPosition( { m_vPosition.x, m_vPosition.y } );
		else if ( m_sType == "Data Slider" )
			m_pDataSlider->GetTransform()->SetPosition( { m_vPosition.x, m_vPosition.y } );
		else if ( m_sType == "Drop Down" )
			m_pDropDown->GetTransform()->SetPosition( { m_vPosition.x, m_vPosition.y } );
		else if ( m_sType == "Energy Bar" )
			m_pEnergyBar->GetTransform()->SetPosition( { m_vPosition.x, m_vPosition.y } );
		else if ( m_sType == "Image" )
			m_pImage->GetTransform()->SetPosition( { m_vPosition.x, m_vPosition.y } );
		else if ( m_sType == "Input" )
			m_pInput->GetTransform()->SetPosition( { m_vPosition.x, m_vPosition.y } );
	}

	inline std::shared_ptr<Button_Widget> GetButtonWidget() const noexcept { return m_pButton; }
	inline std::shared_ptr<ColourBlock_Widget> GetColourBlockWidget() const noexcept { return m_pColourBlock; }
	inline std::shared_ptr<DataSlider_Widget> GetDataSliderWidget() const noexcept { return m_pDataSlider; }
	inline std::shared_ptr<DropDown_Widget> GetDropDownWidget() const noexcept { return m_pDropDown; }
	inline std::shared_ptr<EnergyBar_Widget> GetEnergyBarWidget() const noexcept { return m_pEnergyBar; }
	inline std::shared_ptr<Image_Widget> GetImageWidget() const noexcept { return m_pImage; }
	inline std::shared_ptr<Input_Widget> GetInputWidget() const noexcept { return m_pInput; }

protected:
	int m_iZIndex;
	bool m_bHidden;
	XMFLOAT2 m_vPosition, m_vSize;
	std::string m_sName, m_sType, m_sAction;
	std::shared_ptr<Button_Widget> m_pButton;
	std::shared_ptr<ColourBlock_Widget> m_pColourBlock;
	std::shared_ptr<DataSlider_Widget> m_pDataSlider;
	std::shared_ptr<DropDown_Widget> m_pDropDown;
	std::shared_ptr<EnergyBar_Widget> m_pEnergyBar;
	std::shared_ptr<Image_Widget> m_pImage;
	std::shared_ptr<Input_Widget> m_pInput;
};

#endif