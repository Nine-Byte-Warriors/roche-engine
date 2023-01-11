#pragma once
#ifndef INPUT_H
#define INPUT_H

#include "Camera.h"
#include "WindowContainer.h"
#include <Vector2f.h>

class Input : public WindowContainer
{
public:
	void Initialize( RenderWindow& window );
	void Update( float dt );

	inline Mouse& GetMouse() noexcept { return mouse; }
	inline Keyboard& GetKeyboard() noexcept { return keyboard; }

private:
	void UpdateMouse( float dt );
	void UpdateKeyboard( float dt );

	//DEBUG: for testing only
	Vector2f* pMousePos;
};

#endif