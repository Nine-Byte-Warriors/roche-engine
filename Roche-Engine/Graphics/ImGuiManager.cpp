#include "stdafx.h"
#include "ImGuiManager.h"
#if _DEBUG
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

ImGuiManager::ImGuiManager()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
    ImGui::StyleColorsDark();
    AddToEvent();
}

ImGuiManager::~ImGuiManager()
{
    RemoveFromEvent();
	ImGui::DestroyContext();
}

void ImGuiManager::Initialize( HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* context ) const noexcept
{
    ImGui_ImplWin32_Init( hWnd );
    ImGui_ImplDX11_Init( device, context );

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void ImGuiManager::BeginRender() const noexcept
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport( ImGui::GetMainViewport(),ImGuiDockNodeFlags_PassthruCentralNode );
}

void ImGuiManager::SpawnMenuBar() noexcept
{
    if ( ImGui::BeginMainMenuBar() )
    {
        if ( ImGui::BeginMenu( "Styles" ) )
        {
            bool selected = false;
            static int selectedIdx = 0;
			static std::vector<bool> styleBools = { true, false, false, false, false };

            if ( ImGui::MenuItem( "Dark", NULL, styleBools[0] ) )
            {
                selected = true;
				selectedIdx = 0;
                ImGui::StyleColorsDark();
            }

            if ( ImGui::MenuItem( "Light", NULL, styleBools[1] ) )
            {
                selected = true;
                selectedIdx = 1;
                ImGui::StyleColorsLight();
            }

            if ( ImGui::MenuItem( "Black Gold", NULL, styleBools[2] ) )
            {
                selected = true;
                selectedIdx = 2;
                SetBlackGoldStyle();
            }

            if ( ImGui::MenuItem( "Futuristic", NULL, styleBools[3] ) )
            {
                selected = true;
                selectedIdx = 3;
                SetFuturisticStyle();
            }

            if ( ImGui::MenuItem( "Dracula", NULL, styleBools[4] ) )
            {
                selected = true;
                selectedIdx = 4;
                SetDraculaStyle();
            }

            if ( selected )
            {
                selected = false;
                for ( unsigned int i = 0; i < styleBools.size(); i++ )
                {
                    ( i == selectedIdx ) ?
                        styleBools[i] = true :
                        styleBools[i] = false;
                }
            }

            ImGui::EndMenu();
        }

        static bool showControlsWindow = false;
        static bool showInfoWindow = false;
        if ( ImGui::BeginMenu( "Windows" ) )
        {
            if ( ImGui::MenuItem( "Controls", NULL, showControlsWindow ) )
                showControlsWindow = !showControlsWindow;

            if ( ImGui::MenuItem( "Info", NULL, showInfoWindow ) )
                showInfoWindow = !showInfoWindow;

            ImGui::EndMenu();
        }
        if ( showControlsWindow ) SpawnControlsWindow();
        if ( showInfoWindow ) SpawnInfoWindow();

        static bool openLink = true;
        if ( ImGui::BeginMenu( "Help" ) )
        {
            if ( openLink )
            {
                openLink = false;
                ShellExecute( 0, 0, L"https://github.com/Nine-Byte-Warriors/roche-engine/wiki", 0, 0, SW_SHOW );
            }
            ImGui::EndMenu();
        }
        else
        {
            openLink = true;
        }
        ImGui::EndMainMenuBar();
    }
}

void ImGuiManager::EndRender() const noexcept
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData() );
}

void ImGuiManager::SpawnControlsWindow() noexcept
{
    if ( ImGui::Begin( "Controls" ) )
    {
        ImGui::Text( "WASD\tMove Camera" );
        ImGui::Text( "Home\tEnable Mouse" );
        ImGui::Text( "End \tDisable Mouse" );
        ImGui::Text( "F1  \tEnable Windows" );
        ImGui::Text( "F2  \tDisable Windows" );
        ImGui::Text( "Esc \tClose Application" );
    }
    ImGui::End();
}

void ImGuiManager::SpawnInfoWindow() noexcept
{
    if ( ImGui::Begin( "Info" ) )
    {
	    ImGui::Text( "Roche Engine" );
	    ImGui::Text( "Version: 0.1.0" );
	    ImGui::Text( "Author: Nine Byte Warriors" );
        ImGui::Text( "Github: https://github.com/Nine-Byte-Warriors" );
    }
    ImGui::End();
}

void ImGuiManager::SetBlackGoldStyle() noexcept
{
    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    colors[ImGuiCol_Text]                   = ImVec4( 0.92f, 0.92f, 0.92f, 1.00f );
    colors[ImGuiCol_TextDisabled]           = ImVec4( 0.44f, 0.44f, 0.44f, 1.00f );
    colors[ImGuiCol_WindowBg]               = ImVec4( 0.06f, 0.06f, 0.06f, 1.00f );
    colors[ImGuiCol_ChildBg]                = ImVec4( 0.00f, 0.00f, 0.00f, 0.00f );
    colors[ImGuiCol_PopupBg]                = ImVec4( 0.08f, 0.08f, 0.08f, 0.94f );
    colors[ImGuiCol_Border]                 = ImVec4( 0.51f, 0.36f, 0.15f, 1.00f );
    colors[ImGuiCol_BorderShadow]           = ImVec4( 0.00f, 0.00f, 0.00f, 0.00f );
    colors[ImGuiCol_FrameBg]                = ImVec4( 0.11f, 0.11f, 0.11f, 1.00f );
    colors[ImGuiCol_FrameBgHovered]         = ImVec4( 0.51f, 0.36f, 0.15f, 1.00f );
    colors[ImGuiCol_FrameBgActive]          = ImVec4( 0.78f, 0.55f, 0.21f, 1.00f );
    colors[ImGuiCol_TitleBg]                = ImVec4( 0.51f, 0.36f, 0.15f, 1.00f );
    colors[ImGuiCol_TitleBgActive]          = ImVec4( 0.91f, 0.64f, 0.13f, 1.00f );
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4( 0.00f, 0.00f, 0.00f, 0.51f );
    colors[ImGuiCol_MenuBarBg]              = ImVec4( 0.11f, 0.11f, 0.11f, 1.00f );
    colors[ImGuiCol_ScrollbarBg]            = ImVec4( 0.06f, 0.06f, 0.06f, 0.53f );
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4( 0.21f, 0.21f, 0.21f, 1.00f );
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4( 0.47f, 0.47f, 0.47f, 1.00f );
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4( 0.81f, 0.83f, 0.81f, 1.00f );
    colors[ImGuiCol_CheckMark]              = ImVec4( 0.78f, 0.55f, 0.21f, 1.00f );
    colors[ImGuiCol_SliderGrab]             = ImVec4( 0.91f, 0.64f, 0.13f, 1.00f );
    colors[ImGuiCol_SliderGrabActive]       = ImVec4( 0.91f, 0.64f, 0.13f, 1.00f );
    colors[ImGuiCol_Button]                 = ImVec4( 0.51f, 0.36f, 0.15f, 1.00f );
    colors[ImGuiCol_ButtonHovered]          = ImVec4( 0.91f, 0.64f, 0.13f, 1.00f );
    colors[ImGuiCol_ButtonActive]           = ImVec4( 0.78f, 0.55f, 0.21f, 1.00f );
    colors[ImGuiCol_Header]                 = ImVec4( 0.51f, 0.36f, 0.15f, 1.00f );
    colors[ImGuiCol_HeaderHovered]          = ImVec4( 0.91f, 0.64f, 0.13f, 1.00f );
    colors[ImGuiCol_HeaderActive]           = ImVec4( 0.93f, 0.65f, 0.14f, 1.00f );
    colors[ImGuiCol_Separator]              = ImVec4( 0.21f, 0.21f, 0.21f, 1.00f );
    colors[ImGuiCol_SeparatorHovered]       = ImVec4( 0.91f, 0.64f, 0.13f, 1.00f );
    colors[ImGuiCol_SeparatorActive]        = ImVec4( 0.78f, 0.55f, 0.21f, 1.00f );
    colors[ImGuiCol_ResizeGrip]             = ImVec4( 0.21f, 0.21f, 0.21f, 1.00f );
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4( 0.91f, 0.64f, 0.13f, 1.00f );
    colors[ImGuiCol_ResizeGripActive]       = ImVec4( 0.78f, 0.55f, 0.21f, 1.00f );
    colors[ImGuiCol_Tab]                    = ImVec4( 0.51f, 0.36f, 0.15f, 1.00f );
    colors[ImGuiCol_TabHovered]             = ImVec4( 0.91f, 0.64f, 0.13f, 1.00f );
    colors[ImGuiCol_TabActive]              = ImVec4( 0.78f, 0.55f, 0.21f, 1.00f );
    colors[ImGuiCol_TabUnfocused]           = ImVec4( 0.07f, 0.10f, 0.15f, 0.97f );
    colors[ImGuiCol_TabUnfocusedActive]     = ImVec4( 0.14f, 0.26f, 0.42f, 1.00f );
    colors[ImGuiCol_PlotLines]              = ImVec4( 0.61f, 0.61f, 0.61f, 1.00f );
    colors[ImGuiCol_PlotLinesHovered]       = ImVec4( 1.00f, 0.43f, 0.35f, 1.00f );
    colors[ImGuiCol_PlotHistogram]          = ImVec4( 0.90f, 0.70f, 0.00f, 1.00f );
    colors[ImGuiCol_PlotHistogramHovered]   = ImVec4( 1.00f, 0.60f, 0.00f, 1.00f );
    colors[ImGuiCol_TextSelectedBg]         = ImVec4( 0.26f, 0.59f, 0.98f, 0.35f );
    colors[ImGuiCol_DragDropTarget]         = ImVec4( 1.00f, 1.00f, 0.00f, 0.90f );
    colors[ImGuiCol_NavHighlight]           = ImVec4( 0.26f, 0.59f, 0.98f, 1.00f );
    colors[ImGuiCol_NavWindowingHighlight]  = ImVec4( 1.00f, 1.00f, 1.00f, 0.70f );
    colors[ImGuiCol_NavWindowingDimBg]      = ImVec4( 0.80f, 0.80f, 0.80f, 0.20f );
    colors[ImGuiCol_ModalWindowDimBg]       = ImVec4( 0.80f, 0.80f, 0.80f, 0.35f );

    style->FramePadding = ImVec2( 4.0f, 2.0f );
    style->ItemSpacing = ImVec2( 10.0f, 2.0f );
    style->IndentSpacing = 12.0f;
    style->ScrollbarSize = 10.0f;
    style->WindowRounding = 4.0f;
    style->FrameRounding = 4.0f;
    style->PopupRounding = 4.0f;
    style->ScrollbarRounding = 6.0f;
    style->GrabRounding = 4.0f;
    style->TabRounding = 4.0f;
    style->WindowTitleAlign = ImVec2( 1.0f, 0.5f );
    style->WindowMenuButtonPosition = ImGuiDir_Right;
    style->DisplaySafeAreaPadding = ImVec2( 4.0f, 4.0f );
}

void ImGuiManager::SetFuturisticStyle() noexcept
{
    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    colors[ImGuiCol_Text] = ImVec4( 0.00f, 1.00f, 1.00f, 1.00f );
    colors[ImGuiCol_TextDisabled] = ImVec4( 0.00f, 0.40f, 0.41f, 1.00f );
    colors[ImGuiCol_WindowBg] = ImVec4( 0.00f, 0.00f, 0.00f, 1.00f );
    colors[ImGuiCol_Border] = ImVec4( 0.00f, 1.00f, 1.00f, 0.65f );
    colors[ImGuiCol_BorderShadow] = ImVec4( 0.00f, 0.00f, 0.00f, 0.00f );
    colors[ImGuiCol_FrameBg] = ImVec4( 0.44f, 0.80f, 0.80f, 0.18f );
    colors[ImGuiCol_FrameBgHovered] = ImVec4( 0.44f, 0.80f, 0.80f, 0.27f );
    colors[ImGuiCol_FrameBgActive] = ImVec4( 0.44f, 0.81f, 0.86f, 0.66f );
    colors[ImGuiCol_TitleBg] = ImVec4( 0.14f, 0.18f, 0.21f, 0.73f );
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4( 0.00f, 0.00f, 0.00f, 0.54f );
    colors[ImGuiCol_TitleBgActive] = ImVec4( 0.00f, 1.00f, 1.00f, 0.27f );
    colors[ImGuiCol_MenuBarBg] = ImVec4( 0.00f, 0.00f, 0.00f, 0.20f );
    colors[ImGuiCol_ScrollbarBg] = ImVec4( 0.22f, 0.29f, 0.30f, 0.71f );
    colors[ImGuiCol_ScrollbarGrab] = ImVec4( 0.00f, 1.00f, 1.00f, 0.44f );
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4( 0.00f, 1.00f, 1.00f, 0.74f );
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4( 0.00f, 1.00f, 1.00f, 1.00f );
    colors[ImGuiCol_CheckMark] = ImVec4( 0.00f, 1.00f, 1.00f, 0.68f );
    colors[ImGuiCol_SliderGrab] = ImVec4( 0.00f, 1.00f, 1.00f, 0.36f );
    colors[ImGuiCol_SliderGrabActive] = ImVec4( 0.00f, 1.00f, 1.00f, 0.76f );
    colors[ImGuiCol_Button] = ImVec4( 0.00f, 0.65f, 0.65f, 0.46f );
    colors[ImGuiCol_ButtonHovered] = ImVec4( 0.01f, 1.00f, 1.00f, 0.43f );
    colors[ImGuiCol_ButtonActive] = ImVec4( 0.00f, 1.00f, 1.00f, 0.62f );
    colors[ImGuiCol_Header] = ImVec4( 0.00f, 1.00f, 1.00f, 0.33f );
    colors[ImGuiCol_HeaderHovered] = ImVec4( 0.00f, 1.00f, 1.00f, 0.42f );
    colors[ImGuiCol_HeaderActive] = ImVec4( 0.00f, 1.00f, 1.00f, 0.54f );
    colors[ImGuiCol_ResizeGrip] = ImVec4( 0.00f, 1.00f, 1.00f, 0.54f );
    colors[ImGuiCol_ResizeGripHovered] = ImVec4( 0.00f, 1.00f, 1.00f, 0.74f );
    colors[ImGuiCol_ResizeGripActive] = ImVec4( 0.00f, 1.00f, 1.00f, 1.00f );
    colors[ImGuiCol_PlotLines] = ImVec4( 0.00f, 1.00f, 1.00f, 1.00f );
    colors[ImGuiCol_PlotLinesHovered] = ImVec4( 0.00f, 1.00f, 1.00f, 1.00f );
    colors[ImGuiCol_PlotHistogram] = ImVec4( 0.00f, 1.00f, 1.00f, 1.00f );
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4( 0.00f, 1.00f, 1.00f, 1.00f );
    colors[ImGuiCol_TextSelectedBg] = ImVec4( 0.00f, 1.00f, 1.00f, 0.22f );

    style->Alpha = 1.0;
    style->WindowRounding = 3;
    style->GrabRounding = 1;
    style->GrabMinSize = 20;
    style->FrameRounding = 3;
}

void ImGuiManager::SetDraculaStyle() noexcept
{
    auto& colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.1f, 0.13f, 1.0f };
    colors[ImGuiCol_MenuBarBg] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };

    // Border
    colors[ImGuiCol_Border] = ImVec4{ 0.44f, 0.37f, 0.61f, 0.29f };
    colors[ImGuiCol_BorderShadow] = ImVec4{ 0.0f, 0.0f, 0.0f, 0.24f };

    // Text
    colors[ImGuiCol_Text] = ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f };
    colors[ImGuiCol_TextDisabled] = ImVec4{ 0.5f, 0.5f, 0.5f, 1.0f };

    // Headers
    colors[ImGuiCol_Header] = ImVec4{ 0.13f, 0.13f, 0.17, 1.0f };
    colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.19f, 0.2f, 0.25f, 1.0f };
    colors[ImGuiCol_HeaderActive] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };

    // Buttons
    colors[ImGuiCol_Button] = ImVec4{ 0.13f, 0.13f, 0.17, 1.0f };
    colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.19f, 0.2f, 0.25f, 1.0f };
    colors[ImGuiCol_ButtonActive] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };
    colors[ImGuiCol_CheckMark] = ImVec4{ 0.74f, 0.58f, 0.98f, 1.0f };

    // Popups
    colors[ImGuiCol_PopupBg] = ImVec4{ 0.1f, 0.1f, 0.13f, 0.92f };

    // Slider
    colors[ImGuiCol_SliderGrab] = ImVec4{ 0.44f, 0.37f, 0.61f, 0.54f };
    colors[ImGuiCol_SliderGrabActive] = ImVec4{ 0.74f, 0.58f, 0.98f, 0.54f };

    // Frame BG
    colors[ImGuiCol_FrameBg] = ImVec4{ 0.13f, 0.13, 0.17, 1.0f };
    colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.19f, 0.2f, 0.25f, 1.0f };
    colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };

    // Tabs
    colors[ImGuiCol_Tab] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };
    colors[ImGuiCol_TabHovered] = ImVec4{ 0.24, 0.24f, 0.32f, 1.0f };
    colors[ImGuiCol_TabActive] = ImVec4{ 0.2f, 0.22f, 0.27f, 1.0f };
    colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };

    // Title
    colors[ImGuiCol_TitleBg] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };
    colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };

    // Scrollbar
    colors[ImGuiCol_ScrollbarBg] = ImVec4{ 0.1f, 0.1f, 0.13f, 1.0f };
    colors[ImGuiCol_ScrollbarGrab] = ImVec4{ 0.16f, 0.16f, 0.21f, 1.0f };
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4{ 0.19f, 0.2f, 0.25f, 1.0f };
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4{ 0.24f, 0.24f, 0.32f, 1.0f };

    // Seperator
    colors[ImGuiCol_Separator] = ImVec4{ 0.44f, 0.37f, 0.61f, 1.0f };
    colors[ImGuiCol_SeparatorHovered] = ImVec4{ 0.74f, 0.58f, 0.98f, 1.0f };
    colors[ImGuiCol_SeparatorActive] = ImVec4{ 0.84f, 0.58f, 1.0f, 1.0f };

    // Resize Grip
    colors[ImGuiCol_ResizeGrip] = ImVec4{ 0.44f, 0.37f, 0.61f, 0.29f };
    colors[ImGuiCol_ResizeGripHovered] = ImVec4{ 0.74f, 0.58f, 0.98f, 0.29f };
    colors[ImGuiCol_ResizeGripActive] = ImVec4{ 0.84f, 0.58f, 1.0f, 0.29f };

    // Docking
    colors[ImGuiCol_DockingPreview] = ImVec4{ 0.44f, 0.37f, 0.61f, 1.0f };

    auto& style = ImGui::GetStyle();
    style.TabRounding = 4;
    style.ScrollbarRounding = 9;
    style.WindowRounding = 7;
    style.GrabRounding = 3;
    style.FrameRounding = 3;
    style.PopupRounding = 4;
    style.ChildRounding = 4;
}

void ImGuiManager::AddToEvent() noexcept
{
    EventSystem::Instance()->AddClient( EVENTID::WindowSizeChangeEvent, this );
}

void ImGuiManager::RemoveFromEvent() noexcept
{
    EventSystem::Instance()->RemoveClient( EVENTID::WindowSizeChangeEvent, this );
}

void ImGuiManager::HandleEvent( Event* event )
{
    switch ( event->GetEventID() )
    {
    case EVENTID::WindowSizeChangeEvent :
    {
        m_vWindowSize = *static_cast<XMFLOAT2*>( event->GetData() );
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize.x = m_vWindowSize.x;
        io.DisplaySize.y = m_vWindowSize.y;
    }
    break;
    }
}
#endif