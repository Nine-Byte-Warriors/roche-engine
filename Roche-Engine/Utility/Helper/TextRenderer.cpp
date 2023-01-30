#include "stdafx.h"
#include "TextRenderer.h"

void TextRenderer::Initialize( const std::string& font, ID3D11Device* device, ID3D11DeviceContext* context )
{
	m_sFileName = std::wstring( font.begin(), font.end() );
	m_spriteBatch = std::make_unique<SpriteBatch>( context );
	m_spriteFont = std::make_unique<SpriteFont>( device, std::wstring( L"Resources\\Fonts\\" ).append( m_sFileName ).c_str() );
}

void TextRenderer::DrawString( const std::wstring& text, XMFLOAT2 position, XMVECTORF32 color, bool outline )
{
	m_spriteBatch->Begin();
	XMFLOAT2 originF1 = XMFLOAT2( 0.0f, 0.0f );
	if ( outline )
	{
		std::function<XMFLOAT2( const wchar_t* )> DrawOutline = [&]( const wchar_t* text ) mutable -> XMFLOAT2
		{
			XMFLOAT2 originF2 = XMFLOAT2( 1.0f, 1.0f );
			XMVECTOR origin = m_spriteFont->MeasureString( text ) / 2.0f;
			XMStoreFloat2( &originF2, origin );

			// Draw outline
			m_spriteFont->DrawString( m_spriteBatch.get(), text,
				XMFLOAT2( position.x + 1.0f, position.y + 1.0f ), Colors::Black, 0.0f, originF2 );
			m_spriteFont->DrawString( m_spriteBatch.get(), text,
				XMFLOAT2( position.x - 1.0f, position.y + 1.0f ), Colors::Black, 0.0f, originF2 );
			m_spriteFont->DrawString( m_spriteBatch.get(), text,
				XMFLOAT2( position.x - 1.0f, position.y - 1.0f ), Colors::Black, 0.0f, originF2 );
			m_spriteFont->DrawString( m_spriteBatch.get(), text,
				XMFLOAT2( position.x + 1.0f, position.y - 1.0f ), Colors::Black, 0.0f, originF2 );

			return originF2;
		};
		originF1 = DrawOutline( text.c_str() );
	}
    m_spriteFont->DrawString( m_spriteBatch.get(), text.c_str(), position, color, 0.0f, originF1, m_vScale );
    m_spriteBatch->End();
}

void TextRenderer::RenderString( const std::string& text, XMFLOAT2 position, XMVECTORF32 color, bool outline )
{
	std::wstring stringWide = std::wstring( text.begin(), text.end() );
	DrawString( stringWide, position, color, outline );
}

// Update viewport when screen size changes
void TextRenderer::UpdateViewport( D3D11_VIEWPORT& NewView )
{
	m_spriteBatch->SetViewport( NewView );

	// Scale text
	float xScale = 1, yScale = 1;
	if ( NewView.Width <= 2560 && NewView.Width > 1920 )
		xScale = 2.0f;
	else if ( NewView.Width <= 1920 && NewView.Width > 1600 )
		xScale = 1.5f;
	else if ( NewView.Width <= 1600 && NewView.Width > 1024 )
		xScale = 1.0f;
	else if ( NewView.Width <= 1024 )
		xScale = 0.9f;
	else
		xScale = 1.0f;

	if ( NewView.Height <= 1440 && NewView.Height > 1080 )
		yScale = 2.0f;
	else if ( NewView.Height <= 1080 && NewView.Height > 900 )
		yScale = 1.5f;
	else if ( NewView.Height <= 900 && NewView.Height > 576 )
		yScale = 1.0f;
	else if ( NewView.Height <= 576 )
		yScale = 0.9f;
	else
		yScale = 1.0f;

	SetScale( { xScale, yScale } );
}