#pragma once
#ifndef MOUSEDATA_H
#define MOUSEDATA_H

struct MouseData
{
	MouseData()
		: Pos( { 0.0f, 0.0f } )
		, LPress( false )
		, RPress( false )
		, MPress( false )
		, Locked( false )
		, Hover( false )
	{}

	XMFLOAT2 Pos;
	bool LPress;
	bool RPress;
	bool MPress;
	bool Locked;
	bool Hover;
};

#endif