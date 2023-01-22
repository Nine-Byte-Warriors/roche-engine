#pragma once
#ifndef WIDGET_H
#define WIDGET_H

#include "Sprite.h"
#include "TextRenderer.h"

struct MouseData
{
	MouseData() 
		: Pos( { 0.0f, 0.0f } )
		, LPress( false )
		, RPress( false )
		, MPress( false )
		, Locked( false )
	{}

	XMFLOAT2 Pos;
	bool LPress;
	bool RPress;
	bool MPress;
	bool Locked;
};

class Widget
{
public:
	Widget() {}
	Widget( std::string name, std::string type, std::string action, XMFLOAT2 pos, XMFLOAT2 scale )
		: m_sName( name ), m_sType( type ), m_sAction( action ), m_vPosition( pos ), m_vSize( scale )
	{}
	~Widget() {}

	virtual void Resolve() {}
	virtual void Draw( ID3D11Device* device, ID3D11DeviceContext* context, XMMATRIX worldOrtho ) {}
	inline void IntializeWidget( Widget widget )
	{
		m_sName = widget.GetName();
		m_sType = widget.GetType();
		m_vSize = widget.GetSize();
		m_sAction = widget.GetAction();
		m_vPosition = widget.GetPosition();
	}

	inline XMFLOAT2 GetSize() const noexcept { return m_vSize; }
	inline std::string GetName() const noexcept { return m_sName; }
	inline std::string GetType() const noexcept { return m_sType; }
	inline std::string GetAction() const noexcept { return m_sAction; }
	inline XMFLOAT2 GetPosition() const noexcept { return m_vPosition; }

	inline void SetSize( const XMFLOAT2& size ) noexcept { m_vSize = size; }
	inline void SetName( const std::string& name ) noexcept { m_sName = name; }
	inline void SetType( const std::string& type ) noexcept { m_sType = type; }
	inline void SetPosition( const XMFLOAT2& pos ) noexcept { m_vPosition = pos; }
	inline void SetAction( const std::string& action ) noexcept { m_sAction = action; }

protected:
	XMFLOAT2 m_vPosition, m_vSize;
	std::string m_sName, m_sType, m_sAction;
};

#endif