#include "stdafx.h"
#include "TextRenderer.h"

void TextRenderer::Initialize( ID3D11Device* device, ID3D11DeviceContext* context )
{
	m_spriteBatch = std::make_unique<SpriteBatch>( context );
	std::vector<std::wstring> fontSizes = { L"16", L"20", L"24", L"36", L"72" };
	for ( unsigned int i = 0; i < fontSizes.size(); i++ )
		m_spriteFonts.emplace( (FontSize)i,
			std::make_unique<SpriteFont>(
				device, std::wstring( L"Resources\\Fonts\\midnight_chalker_" ).append( fontSizes.at( i ) ).append( L".spritefont" ).c_str()
			)
		);
}

void TextRenderer::DrawString( const std::wstring& text, XMFLOAT2 position, XMVECTORF32 color, FontSize size, bool outline )
{
	m_spriteBatch->Begin();
	XMFLOAT2 origin = XMFLOAT2( 0.0f, 0.0f );
	if ( outline )
	{
		std::function<void( const wchar_t* )> DrawOutline = [&]( const wchar_t* text ) mutable -> void
		{
			// Draw outline
			m_spriteFonts[size]->DrawString( m_spriteBatch.get(), text,
				XMFLOAT2( position.x + 1.0f, position.y + 1.0f ), Colors::Black, 0.0f, origin );
			m_spriteFonts[size]->DrawString( m_spriteBatch.get(), text,
				XMFLOAT2( position.x - 1.0f, position.y + 1.0f ), Colors::Black, 0.0f, origin );
			m_spriteFonts[size]->DrawString( m_spriteBatch.get(), text,
				XMFLOAT2( position.x - 1.0f, position.y - 1.0f ), Colors::Black, 0.0f, origin );
			m_spriteFonts[size]->DrawString( m_spriteBatch.get(), text,
				XMFLOAT2( position.x + 1.0f, position.y - 1.0f ), Colors::Black, 0.0f, origin );
		};
		DrawOutline( text.c_str() );
	}
    m_spriteFonts[size]->DrawString( m_spriteBatch.get(), text.c_str(), position, color, 0.0f, origin, m_vScale );
    m_spriteBatch->End();
}

void TextRenderer::RenderString( const std::string& text, XMFLOAT2 position, XMVECTORF32 color, FontSize size, bool outline )
{
	std::wstring stringWide = std::wstring( text.begin(), text.end() );
	DrawString( stringWide, position, color, size, outline );
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