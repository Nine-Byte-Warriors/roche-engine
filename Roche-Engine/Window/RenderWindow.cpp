#include "stdafx.h"
#include "Resource.h"
#include "WindowContainer.h"

bool RenderWindow::Initialize( WindowContainer* pWindowContainer, HINSTANCE hInstance, const std::string& windowName, const std::string& windowClass, int width, int height )
{
	AddToEvent();

	// register window class
	m_hInstance = hInstance;
	m_iWidth = width;
	m_iHeight = height;
	m_sWindowTitle = windowName;
	m_wsWindowTitle = StringHelper::StringToWide( windowName );
	m_sWindowClass = windowClass;
	m_wsWindowClass = StringHelper::StringToWide( windowClass );

	cursors.emplace( CursorType::NORMAL, LoadCursor( hInstance, (LPCWSTR)IDC_CURSOR1 ) );
	cursors.emplace( CursorType::LINK, LoadCursor( hInstance, (LPCWSTR)IDC_CURSOR2 ) );
	cursors.emplace( CursorType::PRECISION, LoadCursor( hInstance, (LPCWSTR)IDC_CURSOR3 ) );
	cursors.emplace( CursorType::MOVE, LoadCursor( hInstance, (LPCWSTR)IDC_CURSOR4 ) );
	cursors.emplace( CursorType::HELP, LoadCursor( hInstance, (LPCWSTR)IDC_CURSOR5 ) );
	cursors.emplace( CursorType::TEXT, LoadCursor( hInstance, (LPCWSTR)IDC_CURSOR6 ) );
	cursors.emplace( CursorType::PEN, LoadCursor( hInstance, (LPCWSTR)IDC_CURSOR7 ) );
	cursors.emplace( CursorType::UNAVAILABLE, LoadCursor( hInstance, (LPCWSTR)IDC_CURSOR8 ) );

	RegisterWindowClass();

	int centerScreenX = ( GetSystemMetrics( SM_CXSCREEN ) / 2 ) - ( width / 2 );
	int centerScreenY = ( GetSystemMetrics( SM_CYSCREEN ) / 2 ) - ( height / 2 );

	RECT windowRect;
	windowRect.left = centerScreenX;
	windowRect.top = centerScreenY;
	windowRect.right = windowRect.left + width;
	windowRect.bottom = windowRect.top + height;
	DWORD style = WS_POPUP;//WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME;
	AdjustWindowRect( &windowRect, style, FALSE );

	// create window
	m_hWnd = CreateWindow(
		m_wsWindowClass.c_str(),
		m_wsWindowTitle.c_str(),
		style,
		windowRect.left,
		windowRect.top,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		hInstance,
		pWindowContainer
	);

	if ( m_hWnd == NULL )
	{
		ErrorLogger::Log( GetLastError(), "ERROR::CreateWindow Failed for window: " + m_sWindowTitle );
		return false;
	}

	// show window
	ShowWindow( m_hWnd, SW_SHOW );
	SetForegroundWindow( m_hWnd );
	SetFocus( m_hWnd );

	return true;
}

LRESULT CALLBACK HandleMsgRedirect( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) noexcept
{
	switch ( uMsg )
	{
	case WM_CLOSE:
		DestroyWindow( hWnd );
		return 0;

	// handle all other messages
	default:
	{
		// get ptr to window class
		WindowContainer* const pWindow = reinterpret_cast<WindowContainer*>( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );
		// forward messages to window class
		return pWindow->WindowProc( hWnd, uMsg, wParam, lParam );
	}
	}
}

LRESULT CALLBACK HandleMsgSetup( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch ( uMsg )
	{
	case WM_NCCREATE:
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>( lParam );
		WindowContainer* pWindow = reinterpret_cast<WindowContainer*>( pCreate->lpCreateParams );
		if ( pWindow == nullptr )
		{
			ErrorLogger::Log( "ERROR::Pointer to window container is null during WM_NCCREATE!" );
			exit( -1 );
		}
		SetWindowLongPtr( hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>( pWindow ) );
		SetWindowLongPtr( hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>( HandleMsgRedirect ) );
		return pWindow->WindowProc( hWnd, uMsg, wParam, lParam );
	}

	default:
		return DefWindowProc( hWnd, uMsg, wParam, lParam );
	}
}

bool RenderWindow::ProcessMessages() noexcept
{
	MSG msg;
	ZeroMemory( &msg, sizeof( MSG ) );
	while ( PeekMessage( &msg, m_hWnd, 0u, 0u, PM_REMOVE ) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	// check if the window was closed
	if ( msg.message == WM_NULL )
	{
		if ( !IsWindow( m_hWnd ) )
		{
			m_hWnd = NULL;
			UnregisterClass( m_wsWindowClass.c_str(), m_hInstance );
			return false;
		}
	}

	return true;
}

HWND RenderWindow::GetHWND() const noexcept
{
	return m_hWnd;
}

void RenderWindow::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient( EVENTID::CursorUpdate_Normal, this );
	EventSystem::Instance()->AddClient( EVENTID::CursorUpdate_Link, this );
	EventSystem::Instance()->AddClient( EVENTID::CursorUpdate_Precision, this );
	EventSystem::Instance()->AddClient( EVENTID::CursorUpdate_Move, this );
	EventSystem::Instance()->AddClient( EVENTID::CursorUpdate_Help, this );
	EventSystem::Instance()->AddClient( EVENTID::CursorUpdate_Text, this );
	EventSystem::Instance()->AddClient( EVENTID::CursorUpdate_Pen, this );
	EventSystem::Instance()->AddClient( EVENTID::CursorUpdate_Unavailable, this );
	EventSystem::Instance()->AddClient( EVENTID::WindowSizeChangeEvent, this );
	EventSystem::Instance()->AddClient( EVENTID::QuitGameEvent, this );
}

void RenderWindow::RemoveFromEvent() noexcept
{
	EventSystem::Instance()->RemoveClient( EVENTID::CursorUpdate_Normal, this );
	EventSystem::Instance()->RemoveClient( EVENTID::CursorUpdate_Link, this );
	EventSystem::Instance()->RemoveClient( EVENTID::CursorUpdate_Precision, this );
	EventSystem::Instance()->RemoveClient( EVENTID::CursorUpdate_Move, this );
	EventSystem::Instance()->RemoveClient( EVENTID::CursorUpdate_Help, this );
	EventSystem::Instance()->RemoveClient( EVENTID::CursorUpdate_Text, this );
	EventSystem::Instance()->RemoveClient( EVENTID::CursorUpdate_Pen, this );
	EventSystem::Instance()->RemoveClient( EVENTID::CursorUpdate_Unavailable, this );
	EventSystem::Instance()->RemoveClient( EVENTID::WindowSizeChangeEvent, this );
	EventSystem::Instance()->RemoveClient( EVENTID::QuitGameEvent, this );
}

void RenderWindow::HandleEvent( Event* event )
{
	switch ( event->GetEventID() )
	{
	case EVENTID::CursorUpdate_Normal:
		SetCursor( GetCursor( RenderWindow::CursorType::NORMAL ) );
		break;
	case EVENTID::CursorUpdate_Link:
		SetCursor( GetCursor( RenderWindow::CursorType::LINK ) );
		break;
	case EVENTID::CursorUpdate_Precision:
		SetCursor( GetCursor( RenderWindow::CursorType::PRECISION ) );
		break;
	case EVENTID::CursorUpdate_Move:
		SetCursor( GetCursor( RenderWindow::CursorType::MOVE ) );
		break;
	case EVENTID::CursorUpdate_Help:
		SetCursor( GetCursor( RenderWindow::CursorType::HELP ) );
		break;
	case EVENTID::CursorUpdate_Text:
		SetCursor( GetCursor( RenderWindow::CursorType::TEXT ) );
		break;
	case EVENTID::CursorUpdate_Pen:
		SetCursor( GetCursor( RenderWindow::CursorType::PEN ) );
		break;
	case EVENTID::CursorUpdate_Unavailable:
		SetCursor( GetCursor( RenderWindow::CursorType::UNAVAILABLE ) );
		break;
	case EVENTID::WindowSizeChangeEvent:
		{
			m_bIsStopNextFrame = false;
			XMFLOAT2 sizeOfScreen = *static_cast<XMFLOAT2*>( event->GetData() );
			m_iWidth = sizeOfScreen.x;
			m_iHeight = sizeOfScreen.y;
		}
		break;
	case EVENTID::QuitGameEvent:
		{
			DestroyWindow( m_hWnd );
			PostQuitMessage( 0 );
		}
		break;
	}
}

void RenderWindow::RegisterWindowClass() noexcept
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof( wc );
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = static_cast<HICON>( LoadImage( m_hInstance, MAKEINTRESOURCE( IDI_TUTORIAL1 ), IMAGE_ICON, 32, 32, 0 ) );
    wc.hCursor = cursors[CursorType::NORMAL];
    wc.hbrBackground = (HBRUSH)( COLOR_WINDOW + 1 );
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_wsWindowClass.c_str();
	wc.hIconSm = static_cast< HICON >( LoadImage( m_hInstance, MAKEINTRESOURCE( IDI_TUTORIAL1 ), IMAGE_ICON, 16, 16, 0 ) );
	RegisterClassEx( &wc );
}

RenderWindow::~RenderWindow() noexcept
{
	RemoveFromEvent();
	if ( m_hWnd != NULL )
	{
		UnregisterClass( m_wsWindowClass.c_str(), m_hInstance );
		DestroyWindow( m_hWnd );
	}
}