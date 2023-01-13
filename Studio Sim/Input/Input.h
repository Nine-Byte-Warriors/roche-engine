#pragma once
#ifndef INPUT_H
#define INPUT_H

#include "WindowContainer.h"

class Input : public WindowContainer
{
public:
	void Initialize( RenderWindow& window );
	void Update( float dt );

	inline Mouse& GetMouse() noexcept { return m_mouse; }
	inline Keyboard& GetKeyboard() noexcept { return m_keyboard; }

private:
	void UpdateMouse( float dt );
	void UpdateKeyboard( float dt );
};

#endif