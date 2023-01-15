#pragma once
#ifndef INPUT_H
#define INPUT_H

#include "WindowContainer.h"

class Input : public WindowContainer
{
public:
	void Initialize( RenderWindow& window );
	void Update( const float dt );

	inline Mouse& GetMouse() noexcept { return m_mouse; }
	inline Keyboard& GetKeyboard() noexcept { return m_keyboard; }

private:
	void UpdateMouse( const float dt );
	void UpdateKeyboard( const float dt );
};

#endif