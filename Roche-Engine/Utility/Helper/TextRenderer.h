#pragma once
#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include <dxtk/SpriteFont.h>
#include <dxtk/SpriteBatch.h>

enum class FontSize
{
	VERY_SMALL = 0,
	SMALL,
	MEDIUM,
	LARGE,
	VERY_LARGE
};

/// <summary>
/// Uses SpriteFont and SpriteBatch to draw strings to the screen.
/// Font to be used can be chagned within this classes constructor.
/// </summary>
class TextRenderer
{
public:
	TextRenderer() {}
	~TextRenderer() {}

	void Initialize( ID3D11Device* device, ID3D11DeviceContext* context );
	void RenderString( const std::string& text, XMFLOAT2 position, XMVECTORF32 color, FontSize size, bool outline );
	void UpdateViewport( D3D11_VIEWPORT& viewport );

	inline XMFLOAT2 GetScale() const noexcept { return m_vScale; }
	inline void SetScale( XMFLOAT2 scale ) noexcept { m_vScale = scale; }

	inline SpriteBatch* GetSpriteBatch() const noexcept { return m_spriteBatch.get(); }
	inline SpriteFont* GetSpriteFont( FontSize size ) const noexcept{ return m_spriteFonts.at( size ).get(); }
private:
	void DrawString( const std::wstring& text, XMFLOAT2 position, XMVECTORF32 color, FontSize size, bool outline );

	XMFLOAT2 m_vScale = { 1.0f, 1.0f };
	std::unique_ptr<SpriteBatch> m_spriteBatch;
	// font size, font file path
	std::map<FontSize, std::unique_ptr<SpriteFont>> m_spriteFonts;
};

#endif