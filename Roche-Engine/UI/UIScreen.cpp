#include "stdafx.h"
#include "UIScreen.h"
#include "Graphics.h"
#include <shellapi.h>

#if _DEBUG
extern bool g_bDebug;
#endif
#define RENDER_IF_IN_BOX( x, y, z, code ) if ( x >= y && x <= ( y + z ) ) code

void UIScreen::Initialize( const Graphics& gfx, ConstantBuffer<Matrices>* mat, const std::vector<Widget>& widgets, Health& health )
{
	m_cbMatrices = mat;
	m_vWidgets = widgets;
	m_pPlayerHealth = &health;
	m_pDevice = gfx.GetDevice();
	m_pContext = gfx.GetContext();
	UpdateWidgets();
}

void UIScreen::UpdateWidgets()
{
	int inputIndex = 0;
	for ( unsigned int i = 0; i < m_vWidgets.size(); i++ )
	{
		if ( m_vWidgets[i].GetType() == "Button" )
		{
			Button_Widget button;
			button.Initialize( m_pDevice.Get(), m_pContext.Get(), *m_cbMatrices );
			button.IntializeWidget( m_vWidgets[i] );
			m_vButtons.push_back( std::move( button ) );
		}
		else if ( m_vWidgets[i].GetType() == "Colour Block" )
		{
			ColourBlock_Widget colourBlock;
			colourBlock.Initialize( m_pDevice.Get(), m_pContext.Get(), *m_cbMatrices );
			colourBlock.IntializeWidget( m_vWidgets[i] );
			m_vColourBlocks.push_back( std::move( colourBlock ) );
		}
		else if ( m_vWidgets[i].GetType() == "Data Slider" )
		{
			DataSlider_Widget dataSlider;
			dataSlider.Initialize( m_pDevice.Get(), m_pContext.Get(), *m_cbMatrices );
			dataSlider.IntializeWidget( m_vWidgets[i] );
			m_vDataSliders.push_back( std::move( dataSlider ) );
		}
		else if ( m_vWidgets[i].GetType() == "Drop Down" )
		{
			DropDown_Widget dropDown;
			dropDown.Initialize( m_pDevice.Get(), m_pContext.Get(), *m_cbMatrices );
			dropDown.IntializeWidget( m_vWidgets[i] );
			m_vDropDowns.push_back( std::move( dropDown ) );
		}
		else if ( m_vWidgets[i].GetType() == "Energy Bar" )
		{
			EnergyBar_Widget energyBar;
			energyBar.Initialize( m_pDevice.Get(), m_pContext.Get(), *m_cbMatrices );
			energyBar.IntializeWidget( m_vWidgets[i] );
			m_vEnergyBars.push_back( std::move( energyBar ) );
		}
		else if ( m_vWidgets[i].GetType() == "Image" )
		{
			Image_Widget image;
			image.Initialize( m_pDevice.Get(), m_pContext.Get(), *m_cbMatrices );
			image.IntializeWidget( m_vWidgets[i] );
			m_vImages.push_back( std::move( image ) );
		}
		else if ( m_vWidgets[i].GetType() == "Input" )
		{
			Input_Widget input;
			input.Initialize( m_pDevice.Get(), m_pContext.Get(), *m_cbMatrices, inputIndex );
			input.IntializeWidget( m_vWidgets[i] );
			m_vInputs.push_back( std::move( input ) );
			inputIndex++;
		}
		else if ( m_vWidgets[i].GetType() == "Page Slider" )
		{
			PageSlider_Widget pageSlider;
			pageSlider.Initialize( m_pDevice.Get(), m_pContext.Get(), *m_cbMatrices );
			pageSlider.IntializeWidget( m_vWidgets[i] );
			pageSlider.SetPageSize( m_vScreenSize.y );
			m_vPageSliders.push_back( std::move( pageSlider ) );
		}
	}
}

void UIScreen::Update(const float dt, const std::vector<Widget>& widgets)
{
#pragma region WIDGET_SETUP
	m_vWidgets = widgets;
	m_vButtons.clear();
	m_vColourBlocks.clear();
	m_vDataSliders.clear();
	m_vEnergyBars.clear();
	m_vImages.clear();

	// Save drop down data before clearing
	std::vector<bool> dropDownStates;
	for (unsigned int i = 0; i < m_vDropDowns.size(); i++)
		dropDownStates.push_back( m_vDropDowns[i].GetIsDown() );
	m_vDropDowns.clear();

	// Save input widget data before clearing
	std::vector<bool> inputSelections;
	std::vector<std::string> inputStrings;
	for (unsigned int i = 0; i < m_vInputs.size(); i++)
	{
		inputSelections.push_back(m_vInputs[i].GetSelected());
		inputStrings.push_back(m_vInputs[i].GetCurrentText());
	}
	m_vInputs.clear();

	m_vPageSliders.clear();
	UpdateWidgets();

	// Re-add drop down data
	for (unsigned int i = 0; i < m_vDropDowns.size(); i++)
		m_vDropDowns[i].SetIsDown(dropDownStates[i]);

	// Re-add input widget data
	for (unsigned int i = 0; i < m_vInputs.size(); i++)
	{
		if (inputSelections.size() == i) inputSelections.push_back(false);
		if (inputStrings.size() == i) inputStrings.push_back("");
		m_vInputs[i].SetCurrentText(inputStrings[i]);
		m_vInputs[i].SetSelected(inputSelections[i]);
	}

	if (!m_mouseData.LPress)
		m_mouseData.Locked = false;
#pragma endregion

#pragma region BUTTONS
	for (unsigned int i = 0; i < m_vButtons.size(); i++)
	{
		bool isSeedPacket = false;
		static std::vector<std::string> SeedStrings = { "Carrot", "Bean", "Onion", "Cauliflower", "Potato", "Tomato" };
		for (unsigned int j = 0; j < SeedStrings.size(); j++)
		{
			if (m_vButtons[i].GetAction() == (SeedStrings[j] + " Background"))
			{
				isSeedPacket = true;
				m_vButtons[i].SetTextOffset(XMFLOAT2(40.0f, 30.0f));
				if (m_vButtons[i].Resolve(
					std::to_string(m_inventory.GetActiveSeedPacketCount(SeedStrings[j])),
					Colors::White, m_textures, m_mouseData, m_inventory.IsActiveSeedPacket(i)))
				{
					m_inventory.SetActiveSeedPacket(j);
				}
			}
		}
		if (isSeedPacket)
		{
			// Don't run other button actions if is seed packet
		}
		else if (m_vButtons[i].GetAction() == "Close")
		{
			if (m_vButtons[i].Resolve("Quit Game", Colors::White, m_textures, m_mouseData))
				EventSystem::Instance()->AddEvent(EVENTID::QuitGameEvent);
		}
		else if (m_vButtons[i].GetAction() == "Link")
		{
			if (m_vButtons[i].Resolve("", Colors::White, m_texturesGithub, m_mouseData))
				if (!m_bOpenLink && m_bOpen)
					m_bOpenLink = true;
			if (!m_vButtons[i].GetIsPressed())
				m_bOpen = true;
			if (m_bOpenLink)
			{
				ShellExecute(0, 0, L"https://github.com/Nine-Byte-Warriors", 0, 0, SW_SHOW);
				m_bOpenLink = false;
				m_bOpen = false;
			}
		}
		else if (m_vButtons[i].GetAction() == "Close")
		{
			// Quit Game
			if (m_vButtons[i].Resolve("Quit Game", Colors::White, m_textures, m_mouseData))
				EventSystem::Instance()->AddEvent(EVENTID::QuitGameEvent);
			m_vButtons[i].Update(dt);
		}
		else if (m_vButtons[i].GetAction() == "Start")
		{
			if (m_vButtons[i].Resolve("Start Game", Colors::White, m_textures, m_mouseData))
				EventSystem::Instance()->AddEvent(EVENTID::StartGame);
			m_vButtons[i].Update(dt);
		}
		else if (m_vButtons[i].GetAction() == "Settings")
		{
			if (m_vButtons[i].Resolve("Settings", Colors::White, m_textures, m_mouseData))
				EventSystem::Instance()->AddEvent(EVENTID::OpenSettings);
			m_vButtons[i].Update(dt);
		}
		else if (m_vButtons[i].GetAction() == "Credits")
		{
			if (m_vButtons[i].Resolve("Credits", Colors::White, m_textures, m_mouseData))
				EventSystem::Instance()->AddEvent(EVENTID::OpenCredits);
			m_vButtons[i].Update(dt);
		}
		else if (m_vButtons[i].GetAction() == "Resume")
		{

			if (m_vButtons[i].Resolve("Resume", Colors::White, m_textures, m_mouseData))
				EventSystem::Instance()->AddEvent(EVENTID::ResumeGame);
			m_vButtons[i].Update(dt);
		}
		else if (m_vButtons[i].GetAction() == "Back")
		{
			if (m_vButtons[i].Resolve("Back", Colors::White, m_textures, m_mouseData))
				EventSystem::Instance()->AddEvent(EVENTID::Back);
			m_vButtons[i].Update(dt);
		}
		
		else if (m_vButtons[i].GetAction() == "General")
		{
			if (m_vButtons[i].Resolve("", Colors::White, m_texturesGeneralTabs, m_mouseData))
			{
				EventSystem::Instance()->AddEvent(EVENTID::GeneralTab);
			}
			m_vButtons[i].Update(dt);
		}
		else if (m_vButtons[i].GetAction() == "Graphics")
		{
			if (m_vButtons[i].Resolve("", Colors::White, m_texturesGraphicsTabs, m_mouseData))
				EventSystem::Instance()->AddEvent(EVENTID::GrahpicsTab);
			m_vButtons[i].Update(dt);
		}
		else if (m_vButtons[i].GetAction() == "Music")
		{
			if (m_vButtons[i].Resolve("", Colors::White, m_texturesMusicTabs, m_mouseData))
				EventSystem::Instance()->AddEvent(EVENTID::MusicTab);
			m_vButtons[i].Update(dt);
		}
		else if (m_vButtons[i].GetAction() == "Control")
		{
			if (m_vButtons[i].Resolve("", Colors::White, m_texturesControlTabs, m_mouseData))
				EventSystem::Instance()->AddEvent(EVENTID::ControlTab);
			m_vButtons[i].Update(dt);
		}
		else
		{
			m_vButtons[i].Resolve("", Colors::White, m_textures, m_mouseData);
		}
		m_vButtons[i].Update(dt);
	}
#pragma endregion

#pragma region COLOUR_BLOCKS
	for (unsigned int i = 0; i < m_vColourBlocks.size(); i++)
	{
		// Doesn't need actions
		m_vColourBlocks[i].Resolve({ 210, 210, 150 });
		m_vColourBlocks[i].Update(dt);
	}
#pragma endregion

#pragma region DATA_SLIDERS
	for (unsigned int i = 0; i < m_vDataSliders.size(); i++)
	{
		if (m_vDataSliders[i].GetAction() == "Master Volume")
		{
			// Create a slider that syncs with master volume
			m_vDataSliders[i].Resolve(m_iSliderStart, "Resources\\Textures\\UI\\Slider\\Slider Background.png",
				"Resources\\Textures\\UI\\Slider\\Control Point.png", m_mouseData);; // temp
		}
		else  if ( m_vDataSliders[i].GetAction() == "Music Volume" )
		{
			// Create a slider that syncs with musics volume
			m_vDataSliders[i].Resolve(m_iSliderStart, "Resources\\Textures\\UI\\Slider\\Slider Background.png",
				"Resources\\Textures\\UI\\Slider\\Control Point.png", m_mouseData);; // temp
		}
		else if (m_vDataSliders[i].GetAction() == "Screen Shake")
		{
			// Create a slider that syncs with master volume`
			m_vDataSliders[i].Resolve(m_iSliderStart, "Resources\\Textures\\UI\\Slider\\Slider Background.png",
				"Resources\\Textures\\UI\\Slider\\Control Point.png", m_mouseData);; // temp
		}
		else
		{
			// Default
			m_vDataSliders[i].Resolve(m_iSliderStart, "Resources\\Textures\\UI\\Slider\\Slider Background.png",
				"Resources\\Textures\\UI\\Slider\\Control Point.png", m_mouseData);;
		}
		m_vDataSliders[i].Update(dt);
	}
#pragma endregion

#pragma region DROP_DOWNS
	for (unsigned int i = 0; i < m_vDropDowns.size(); i++)
	{
		if (m_vDropDowns[i].GetAction() == "Resolution")
		{
			// Create a drop down that allows user to change resolution
		}
		else if (m_vDropDowns[i].GetAction() == "Language")
		{
			// Create a drop down that allows user to change Language
			std::vector<std::string> vValues = { "English (UK)", "English (USA)"};
			static std::string sValue = vValues[0];
			m_vDropDowns[i].Resolve(vValues, m_texturesDD, m_texturesDDButton, Colors::White, sValue, m_mouseData);
			if (m_vDropDowns[i].GetSelected() == "English (USA)")
				sValue = "English (USA)";
			else
				sValue = "English (UK)";
		}
		else
		{
			// Default
			std::vector<std::string> vValues = { "True", "False" };
			static std::string sValue = vValues[0];
			m_vDropDowns[i].Resolve(vValues, m_texturesDD, m_texturesDDButton, Colors::White, sValue, m_mouseData);
			if (m_vDropDowns[i].GetSelected() == "False")
				sValue = "False";
			else
				sValue = "True";
		}
		m_vDropDowns[i].Update(dt);
	}
#pragma endregion

#pragma region ENERGY_BARS
	for (unsigned int i = 0; i < m_vEnergyBars.size(); i++)
	{
		if (m_vEnergyBars[i].GetAction() == "Player Health")
		{
			// Bar that displays the player's health
		}
		else if (m_vEnergyBars[i].GetAction() == "Enemy Health")
		{
			// Bar that displays an enemy's health
		}
		else
		{
			// Default
			static float health = 100.0f;
			std::string temp = m_textures[2];
			m_textures[2] = "";
			m_vEnergyBars[i].Resolve(m_textures, health);
			m_textures[2] = temp;
			m_vEnergyBars[i].Update(dt);
		}
	}
#pragma endregion

#pragma region IMAGES
	for (unsigned int i = 0; i < m_vImages.size(); i++)
	{
		if (m_vImages[i].GetAction() == "Master volume label")
		{
			m_vImages[i].Resolve("Master Volumne", Colors::AntiqueWhite, "Resources\\Textures\\Tiles\\transparent.png");
		}
		else if (m_vImages[i].GetAction() == "Music volume label")
		{
			m_vImages[i].Resolve("Music Volumne", Colors::AntiqueWhite, "Resources\\Textures\\Tiles\\transparent.png");
		}

		else if (m_vImages[i].GetAction() == "Coins")
		{
			m_vImages[i].Resolve("0000000", Colors::AntiqueWhite, "Resources\\Textures\\Tiles\\transparent.png");
		}
		else if (m_vImages[i].GetAction() == "Score Label")
		{
			m_vImages[i].Resolve("Score", Colors::AntiqueWhite, "Resources\\Textures\\Tiles\\transparent.png");
		}
		else if (m_vImages[i].GetAction() == "Score")
		{
			m_vImages[i].Resolve("0000000", Colors::AntiqueWhite, "Resources\\Textures\\Tiles\\transparent.png");
		}


		else if (m_vImages[i].GetAction() == "Language DD Label")
		{
			m_vImages[i].Resolve("Language", Colors::AntiqueWhite, "Resources\\Textures\\Tiles\\transparent.png");
		}
		else if (m_vImages[i].GetAction() == "Screen Shake Slider Label")
		{
			m_vImages[i].Resolve("Screen Shake Amount", Colors::AntiqueWhite, "Resources\\Textures\\Tiles\\transparent.png");
		}

		else if (m_vImages[i].GetAction() == "Pause Title")
		{
			m_vImages[i].Resolve("PAUSED", Colors::AntiqueWhite, "Resources\\Textures\\Tiles\\transparent.png", FontSize::INSANE);
		}
		else if (m_vImages[i].GetAction() == "Up Control")
		{
			m_vImages[i].Resolve("Up Control ", Colors::AntiqueWhite, "Resources\\Textures\\Tiles\\transparent.png");
		}
		else if (m_vImages[i].GetAction() == "Left Control")
		{
			m_vImages[i].Resolve("Left Control", Colors::AntiqueWhite, "Resources\\Textures\\Tiles\\transparent.png");
		}
		else if (m_vImages[i].GetAction() == "Down Control")
		{
			m_vImages[i].Resolve("Down Control", Colors::AntiqueWhite, "Resources\\Textures\\Tiles\\transparent.png");
		}
		else if (m_vImages[i].GetAction() == "Right Control")
		{
			m_vImages[i].Resolve("Right Control", Colors::AntiqueWhite, "Resources\\Textures\\Tiles\\transparent.png");
		}
		else if (m_vImages[i].GetAction() == "Shoot Control")
		{
			m_vImages[i].Resolve("Shoot Control", Colors::AntiqueWhite, "Resources\\Textures\\Tiles\\transparent.png");
		}

		else if (m_vImages[i].GetAction() == "CreditTitle")
		{
			m_vImages[i].Resolve("CREDITS", Colors::AntiqueWhite, "Resources\\Textures\\Tiles\\transparent.png");
		}
		else if (m_vImages[i].GetAction() == "Credit1")
		{
			m_vImages[i].Resolve("Kyle Robinson", Colors::AntiqueWhite, "Resources\\Textures\\Tiles\\transparent.png");
		}
		else if (m_vImages[i].GetAction() == "Credit2")
		{
			m_vImages[i].Resolve("Jukiusz Jaczmarek", Colors::AntiqueWhite, "Resources\\Textures\\Tiles\\transparent.png");
		}
		else if (m_vImages[i].GetAction() == "Credit3")
		{
			m_vImages[i].Resolve("Rece Thompson-Hamilton", Colors::AntiqueWhite, "Resources\\Textures\\Tiles\\transparent.png");
		}
		else if (m_vImages[i].GetAction() == "Credit4")
		{
			m_vImages[i].Resolve("Charlie Morris", Colors::AntiqueWhite, "Resources\\Textures\\Tiles\\transparent.png");
		}
		else if (m_vImages[i].GetAction() == "Credit5")
		{
			m_vImages[i].Resolve("Alvin Aggrey", Colors::AntiqueWhite, "Resources\\Textures\\Tiles\\transparent.png");
		}
		else if (m_vImages[i].GetAction() == "Credit6")
		{
			m_vImages[i].Resolve("Will Bennett", Colors::AntiqueWhite, "Resources\\Textures\\Tiles\\transparent.png");
		}
		else if (m_vImages[i].GetAction() == "Credit7")
		{
			m_vImages[i].Resolve("James Barber", Colors::AntiqueWhite, "Resources\\Textures\\Tiles\\transparent.png");
		}
		else if (m_vImages[i].GetAction() == "Credit8")
		{
			m_vImages[i].Resolve("Said Mozamil Sadat", Colors::AntiqueWhite, "Resources\\Textures\\Tiles\\transparent.png");
		}
		else if (m_vImages[i].GetAction() == "Credit9")
		{
			m_vImages[i].Resolve("Glen Nelson", Colors::AntiqueWhite, "Resources\\Textures\\Tiles\\transparent.png");
		}
		else if (m_vImages[i].GetAction() == "Credit10")
		{
			m_vImages[i].Resolve("Eleftherios Karakyritsis", Colors::AntiqueWhite, "Resources\\Textures\\Tiles\\transparent.png");
		}
		else
		{
			std::string currTexture = m_texturesHearts[0];
			float currHealth = m_pPlayerHealth->GetCurrentHealth();
			float maxHealth = m_pPlayerHealth->GetMaxHealth();

			m_vImages[i].Resolve("", Colors::AntiqueWhite, "Resources\\Textures\\UI\\Board\\Board.png");
			if (m_vImages[i].GetAction() == "Carrot Seed Packet")
			{
				m_vImages[i].Resolve("", Colors::AntiqueWhite, "Resources\\Textures\\UI\\Seeds\\Carrot Seeds.png");
			}
			else if (m_vImages[i].GetAction() == "Cauliflower Seed Packet")
			{
				m_vImages[i].Resolve("", Colors::AntiqueWhite, "Resources\\Textures\\UI\\Seeds\\Cauliflower Seeds.png");
			}
			else if (m_vImages[i].GetAction() == "Bean Seed Packet")
			{
				m_vImages[i].Resolve("", Colors::AntiqueWhite, "Resources\\Textures\\UI\\Seeds\\Bean Seeds.png");
			}
			else if (m_vImages[i].GetAction() == "Onion Seed Packet")
			{
				m_vImages[i].Resolve("", Colors::AntiqueWhite, "Resources\\Textures\\UI\\Seeds\\Onion Seeds.png");
			}
			else if (m_vImages[i].GetAction() == "Potato Seed Packet")
			{
				m_vImages[i].Resolve("", Colors::AntiqueWhite, "Resources\\Textures\\UI\\Seeds\\Potato Seeds.png");
			}
			else if (m_vImages[i].GetAction() == "Tomato Seed Packet")
			{
				m_vImages[i].Resolve("", Colors::AntiqueWhite, "Resources\\Textures\\UI\\Seeds\\Tomato Seeds.png");
			}
			else if (m_vImages[i].GetAction() == "Heart_1")
			{
				switch ( (int)currHealth )
				{
				case 0: currTexture = m_texturesHearts[2]; break;
				case 1: currTexture = m_texturesHearts[1]; break;
				case 2: case 3: case 4: case 5: case 6: currTexture = m_texturesHearts[0]; break;
				}
				m_vImages[i].Resolve("", Colors::AntiqueWhite, currTexture);
			}
			else if (m_vImages[i].GetAction() == "Heart_2")
			{
				switch ( (int)currHealth )
				{
				case 0: case 1: case 2: currTexture = m_texturesHearts[2]; break;
				case 3: currTexture = m_texturesHearts[1]; break;
				case 4: case 5: case 6: currTexture = m_texturesHearts[0]; break;
				}
				m_vImages[i].Resolve("", Colors::AntiqueWhite, currTexture);
			}
			else if (m_vImages[i].GetAction() == "Heart_3")
			{
				switch ( (int)currHealth )
				{
				case 0: case 1: case 2: case 3: case 4: currTexture = m_texturesHearts[2]; break;
				case 5: currTexture = m_texturesHearts[1]; break;
				case 6: currTexture = m_texturesHearts[0]; break;
				}
				m_vImages[i].Resolve("", Colors::AntiqueWhite, currTexture);
			}
			else if (m_vImages[i].GetAction() == "Coin Icon")
			{
				m_vImages[i].Resolve("", Colors::AntiqueWhite, "Resources\\Textures\\UI\\Coin\\Coin.png");
			}
			else if (m_vImages[i].GetAction() == "TitleCard")
			{
				m_vImages[i].Resolve("", Colors::AntiqueWhite, "Resources\\Textures\\UI\\Title\\Title.png");
			}
			else
			{
				m_vImages[i].Resolve("", Colors::AntiqueWhite, "Resources\\Textures\\UI\\Board\\Board.png");
			}
			m_vImages[i].Update(dt);
		}
#pragma endregion

#pragma region INPUTS
		for (unsigned int i = 0; i < m_vInputs.size(); i++)
		{
			if (m_vInputs[i].GetAction() == "Player Name")
			{
				// Input that allows the user to enter their name
			}
			else
			{
				// Default
				m_vInputs[i].Resolve(m_sKeys, Colors::White, m_textures, m_mouseData, m_iInputIndex);
				m_vInputs[i].Update(dt);
			}
		}
#pragma endregion

#pragma region PAGE_SLIDERS
		for (unsigned int i = 0; i < m_vPageSliders.size(); i++)
		{
			m_vPageSliders[i].Resolve(Colour(10.0f, 10.0f, 10.0f), Colour(60.0f, 60.0f, 60.0f), m_mouseData);
			m_vPageSliders[i].SetPosition({ m_vScreenSize.x - 30.0f, m_vScreenSize.y * 0.2f });
			m_vPageSliders[i].SetSize({ 30.0f, m_vScreenSize.y * 0.6f });
			m_vPageSliders[i].Update(dt);
			m_fCurrentY = (m_vScreenSize.y * 0.2f) - m_vPageSliders[i].GetPagePos();
			if (m_fCurrentPY != m_vPageSliders[i].GetPY())
			{
				m_fCurrentPY = m_vPageSliders[i].GetPY();
				m_bLoadFlag = true;
			}
			if (m_bUpdateSlider)
				m_vPageSliders[i].SetPageSize(m_vScreenSize.y);
		}
#pragma endregion

#pragma region RENDER_BOX
		// Update render box
		if (m_vPageSliders.size() > 0)
		{
			m_fBoxPos = { 0.0f, m_vScreenSize.y * 0.1f };
			m_fBoxSize = { m_vScreenSize.x, m_vScreenSize.y * 0.6f };
		}
		else
		{
			m_fBoxPos = { 0.0f, 0.0f };
			m_fBoxSize = { m_vScreenSize.x, m_vScreenSize.y };
		}

#pragma endregion
	}
}

void UIScreen::Draw(VertexShader & vtx, PixelShader & pix, XMMATRIX worldOrtho, TextRenderer * textRenderer)
{
	unsigned int widgetAmount =
		m_vButtons.size() + m_vColourBlocks.size() +
		m_vDataSliders.size() + m_vDropDowns.size() + m_vInputs.size() +
		m_vEnergyBars.size() + m_vImages.size() + m_vPageSliders.size();

	for (unsigned int i = 0; i < widgetAmount; i++)
	{
#pragma region BUTTONS
		for (unsigned int j = 0; j < m_vButtons.size(); j++)
		{
			if (m_vButtons[j].GetZIndex() == i)
			{
				if (!m_vButtons[j].GetIsHidden())
				{
					RENDER_IF_IN_BOX(m_vButtons[j].GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y,
						m_vButtons[j].Draw(m_pDevice.Get(), m_pContext.Get(), worldOrtho, textRenderer));
					Shaders::BindShaders(m_pContext.Get(), vtx, pix);
				}
				break;
			}
		}
#pragma endregion

#pragma region COLOUR_BLOCKS
		for (unsigned int j = 0; j < m_vColourBlocks.size(); j++)
		{
			if (m_vColourBlocks[j].GetZIndex() == i)
			{
				if (!m_vColourBlocks[j].GetIsHidden())
				{
					RENDER_IF_IN_BOX(m_vColourBlocks[j].GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y,
						m_vColourBlocks[j].Draw(m_pDevice.Get(), m_pContext.Get(), worldOrtho));
				}
				break;
			}
		}
#pragma endregion

#pragma region DATA_SLIDERS
		for (unsigned int j = 0; j < m_vDataSliders.size(); j++)
		{
			if (m_vDataSliders[j].GetZIndex() == i)
			{
				if (!m_vDataSliders[j].GetIsHidden())
				{
					RENDER_IF_IN_BOX(m_vDataSliders[j].GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y,
						m_vDataSliders[j].Draw(m_pDevice.Get(), m_pContext.Get(), worldOrtho));
				}
				break;
			}
		}
#pragma endregion

#pragma region DROP_DOWNS
		for (unsigned int j = 0; j < m_vDropDowns.size(); j++)
		{
			if (m_vDropDowns[j].GetZIndex() == i)
			{
				if (!m_vDropDowns[j].GetIsHidden())
				{
					RENDER_IF_IN_BOX(m_vDropDowns[j].GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y,
						m_vDropDowns[j].Draw(m_pDevice.Get(), m_pContext.Get(), worldOrtho, textRenderer, vtx, pix));
					Shaders::BindShaders(m_pContext.Get(), vtx, pix);
				}
				break;
			}
		}
#pragma endregion

#pragma region ENERGY_BARS
		for (unsigned int j = 0; j < m_vEnergyBars.size(); j++)
		{
			if (m_vEnergyBars[j].GetZIndex() == i)
			{
				if (!m_vEnergyBars[j].GetIsHidden())
				{
					RENDER_IF_IN_BOX(m_vEnergyBars[j].GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y,
						m_vEnergyBars[j].Draw(m_pDevice.Get(), m_pContext.Get(), worldOrtho));
				}
				break;
			}
		}
#pragma endregion

#pragma region IMAGES
		for (unsigned int j = 0; j < m_vImages.size(); j++)
		{
			if (m_vImages[j].GetZIndex() == i)
			{
				if (!m_vImages[j].GetIsHidden())
				{
					RENDER_IF_IN_BOX(m_vImages[j].GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y,
						m_vImages[j].Draw(m_pDevice.Get(), m_pContext.Get(), worldOrtho, textRenderer));
					Shaders::BindShaders(m_pContext.Get(), vtx, pix);
				}
				break;
			}
		}
#pragma endregion

#pragma region INPUTS
		for (unsigned int j = 0; j < m_vInputs.size(); j++)
		{
			if (m_vInputs[j].GetZIndex() == i)
			{
				if (!m_vInputs[j].GetIsHidden())
				{
					RENDER_IF_IN_BOX(m_vInputs[j].GetTransform()->GetPosition().y, m_fBoxPos.y, m_fBoxSize.y,
						m_vInputs[j].Draw(m_pDevice.Get(), m_pContext.Get(), worldOrtho, textRenderer));
					Shaders::BindShaders(m_pContext.Get(), vtx, pix);
				}
				break;
			}
		}
#pragma endregion

#pragma region PAGE_SLIDERS
		for (unsigned int j = 0; j < m_vPageSliders.size(); j++)
		{
			if (m_vPageSliders[j].GetZIndex() == i)
			{
				if (!m_vPageSliders[j].GetIsHidden())
				{
					RENDER_IF_IN_BOX(m_vPageSliders[j].GetPosition().y, m_fBoxPos.y, m_fBoxSize.y,
						m_vPageSliders[j].Draw(m_pDevice.Get(), m_pContext.Get(), worldOrtho));
				}
				break;
			}
		}
#pragma endregion
	}
}


void UIScreen::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient( EVENTID::KeyInput, this );
#if _DEBUG
	EventSystem::Instance()->AddClient( EVENTID::ImGuiMousePosition, this );
#endif
	EventSystem::Instance()->AddClient( EVENTID::MousePosition, this );
	EventSystem::Instance()->AddClient( EVENTID::LeftMouseClick, this );
	EventSystem::Instance()->AddClient( EVENTID::LeftMouseRelease, this );
	EventSystem::Instance()->AddClient( EVENTID::RightMouseClick, this );
	EventSystem::Instance()->AddClient( EVENTID::RightMouseRelease, this );
	EventSystem::Instance()->AddClient( EVENTID::MiddleMouseClick, this );
	EventSystem::Instance()->AddClient( EVENTID::MiddleMouseRelease, this );
	EventSystem::Instance()->AddClient( EVENTID::WindowSizeChangeEvent, this );
}

void UIScreen::RemoveFromEvent() noexcept
{
	EventSystem::Instance()->RemoveClient( EVENTID::KeyInput, this );
#if _DEBUG
	EventSystem::Instance()->RemoveClient( EVENTID::ImGuiMousePosition, this );
#endif
	EventSystem::Instance()->RemoveClient( EVENTID::MousePosition, this );
	EventSystem::Instance()->RemoveClient( EVENTID::LeftMouseClick, this );
	EventSystem::Instance()->RemoveClient( EVENTID::LeftMouseRelease, this );
	EventSystem::Instance()->RemoveClient( EVENTID::RightMouseClick, this );
	EventSystem::Instance()->RemoveClient( EVENTID::RightMouseRelease, this );
	EventSystem::Instance()->RemoveClient( EVENTID::MiddleMouseClick, this );
	EventSystem::Instance()->RemoveClient( EVENTID::MiddleMouseRelease, this );
	EventSystem::Instance()->RemoveClient( EVENTID::WindowSizeChangeEvent, this );
}

void UIScreen::HandleEvent(Event* event)
{
	switch (event->GetEventID())
	{
	case EVENTID::KeyInput: { m_sKeys = *(std::string*)event->GetData(); } break;
	case EVENTID::LeftMouseClick: { m_mouseData.LPress = true; } break;
	case EVENTID::LeftMouseRelease: { m_mouseData.LPress = false; } break;
	case EVENTID::RightMouseClick: { m_mouseData.RPress = true; } break;
	case EVENTID::RightMouseRelease: { m_mouseData.RPress = false; } break;
	case EVENTID::MiddleMouseClick: { m_mouseData.MPress = true; } break;
	case EVENTID::MiddleMouseRelease: { m_mouseData.MPress = false; } break;
#if _DEBUG
	case EVENTID::ImGuiMousePosition:
	{
		if (!g_bDebug) return;
		Vector2f mousePos = *(Vector2f*)event->GetData();
		m_mouseData.Pos = XMFLOAT2(mousePos.x, mousePos.y);
	}
	break;
#endif
	case EVENTID::MousePosition:
	{
#if _DEBUG
		if (g_bDebug) return;
#endif
		m_mouseData.Pos = *(XMFLOAT2*)event->GetData();
	}
	break;
	case EVENTID::WindowSizeChangeEvent:
	{
		m_vScreenSize = *static_cast<XMFLOAT2*>(event->GetData());
		m_bUpdateSlider = true;
	}
	break;
	}
}