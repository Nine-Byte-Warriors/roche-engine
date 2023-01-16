#include "stdafx.h"
#include "DropDown_Widget.h"

DropDown_Widget::DropDown_Widget( const std::vector<std::string>& ddList, XMFLOAT2 pos, XMFLOAT2 size, std::vector<std::string> backCol, std::vector<std::string> buttonImg, XMVECTORF32 textColour, std::string currData, MouseData mData )
{
	m_spriteBack = std::make_shared<Sprite>();
    m_transformBack = std::make_shared<Transform>( m_spriteBack );

	m_spriteOptions = std::make_shared<Sprite>();
    m_transformOptions = std::make_shared<Transform>( m_spriteOptions );

	Resolve( ddList, pos, size, backCol, buttonImg, textColour, currData, mData );
}

DropDown_Widget::~DropDown_Widget() { }

void DropDown_Widget::Initialize( ID3D11Device* device, ID3D11DeviceContext* context, ConstantBuffer<Matrices>& mat )
{
	m_iFlag = m_iFlagMax;

	m_spriteBack->Initialize( device, context, "", mat, m_vSize.x, m_vSize.y );
    m_transformBack->SetPositionInit( m_vPosition.x, m_vPosition.y );
	m_transformBack->SetScaleInit( m_vSize.x, m_vSize.y );

	m_spriteOptions->Initialize( device, context, "", mat, m_vSize.x, m_vSize.y );
    m_transformOptions->SetPositionInit( m_vPosition.x, m_vPosition.y );
	m_transformOptions->SetScaleInit( m_vSize.x, m_vSize.y );

	m_wButtonDrop.Initialize( device, context, mat );
	for ( unsigned int i = 0; i < ARRAYSIZE( m_wListButtons ); i++ )
		m_wListButtons[i].Initialize( device, context, mat );
}

void DropDown_Widget::Update( const float dt )
{
	m_spriteBack->Update( dt );
	m_transformBack->Update();

	m_spriteOptions->Update( dt );
	m_transformOptions->Update();

	m_wButtonDrop.Update( dt );
}

void DropDown_Widget::Draw(ID3D11DeviceContext* Contex, ID3D11Device* Device, ConstantBuffer<CB_PS_scene>& cb_ps_scene, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d, XMMATRIX WorldOrthoMatrix, TextRenderer* textrender, VertexShader& vert, PixelShader& pix)
{
	_Background.UpdateTex(Device, "Resources\\Textures\\Settings\\Input_Yellow.dds");
	_Background.SetScale(_Size.x, _Size.y);
	_Background.SetInitialPosition(_Pos.x, _Pos.y, 0);

	cb_ps_scene.data.alphaFactor = _AlphaFactor;
	cb_ps_scene.data.useTexture = true;

	if (!cb_ps_scene.ApplyChanges()) return;
	Contex->PSSetConstantBuffers(1u, 1u, cb_ps_scene.GetAddressOf());
	_Background.Draw(WorldOrthoMatrix);
	
	

	ButtonDrop.Draw(Contex, Device, cb_ps_scene, cb_vs_matrix_2d, WorldOrthoMatrix, textrender);
	Shaders::BindShaders(Contex, vert, pix);
	if (DropState== DropState::Down)
	{
		
		for (int i = 0; i < _ListData.size(); i++)
		{


			ListButtons[i].Draw(Contex, Device, cb_ps_scene, cb_vs_matrix_2d, WorldOrthoMatrix, textrender);
			Shaders::BindShaders(Contex, vert, pix);
		}
		
	}
	
	XMVECTOR textsize = textrender->GetSpriteFont()->MeasureString(_ListData[Selected].c_str());
	XMFLOAT2 textpos = { _Pos.x + (_Size.x / 2) - (DirectX::XMVectorGetX(textsize) * textrender->GetScale().x) / 2 ,_Pos.y + (_Size.y / 2) - (DirectX::XMVectorGetY(textsize) * textrender->GetScale().y) / 2 };
	//text	
	textrender->RenderString(_ListData[Selected], textpos, TextColour);
	

}

void DropDown_Widget::Resolve( const std::vector<std::string>& ddList, XMFLOAT2 pos, XMFLOAT2 size, std::vector<std::string> backCol, std::vector<std::string> buttonImg, XMVECTORF32 textColour, std::string currData, MouseData mData )
{
	
	BackgroundColour = Backcolour[2];
	_Size = size;
	_Pos = pos;
	_ListData = DropDownList;
	TextColour = textColour;
	ButtonDrop.Function("", ButtonImage, { size.y, size.y }, XMFLOAT2{ pos.x + size.x ,  pos.y }, textColour, MData);

	for (UINT i = 0; i < _ListData.size(); i++)
	{
		if (Current == _ListData[i]) {
			Selected = i;
		}
	}

	//list buttons
	switch (DropState)
	{
	case DropState::Down: {
		float PosY = pos.y + size.y;
		for (int i = 0; i < DropDownList.size(); i++)
		{
		
			
			if (ListButtons[i].Function(_ListData[i], Backcolour, { size.x,size.y }, XMFLOAT2{ pos.x  ,  PosY }, textColour, MData)) {
				Selected = i;
				DropState = Up;
				Flag = 0;
			}
			PosY += size.y+1;
		}

		if (ButtonDrop.GetIsPressed() && Flag == FlagMax) {

			DropState = Up;
			Flag = 0;
		}
		else if(Flag<FlagMax)
		{
			Flag++;
		}
	}
			 break;
	case DropState::Up:
		if (ButtonDrop.GetIsPressed() && Flag == FlagMax) {

			DropState = Down;
			Flag = 0;
		}
		else if (Flag < FlagMax)
		{
			Flag++;
		}

	default:
		break;
	}

	DataSelected = DropDownList[Selected];
}