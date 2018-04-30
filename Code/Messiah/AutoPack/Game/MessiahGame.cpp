// MessiahGame.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "AutoPack/App/Application.h"
#include "AutoPack/Game/MessiahGame.h"
#include "Foundation/String/String.h"
#include <d3d11.h>


// CreateViewWindow
//------------------------------------------------------------------------------
bool MessiahGame::LoadEmbeddedGame()
{
	if (m_EmbeddedGame) return true;

	// create messiah game
	AStackString<> mainDir = "I:/Projects/H42/code/src/TXM";
	AStackString<> dllDirectory = mainDir; dllDirectory += "/Engine/Binaries/Win64";
	AStackString<> scriptPath = mainDir; scriptPath += "/Package/Script/Python";
	AStackString<> args = "--fs-root="; args += mainDir;
	args += ",--console,--dx11,--load=Python,--start=Python,--suppress=Shepherd,--suppress=Fusion,--suppress=Helium,--suppress=ResourceExpress";

	m_EmbeddedGame = (Module)Platform::LoadDynamicLibrary("EmbeddedClient.2017_x64h.dll", dllDirectory.Get());
	if (!m_EmbeddedGame)
	{
		LERROR("MainView", "Load messiah library failed: %d!\n", Platform::GetLastErrorNo());
		return false;
	}

	if (!m_Wnd)
	{
		CreateViewWindow();
	}

	*(void**)&m_InitDll = ::GetProcAddress((HMODULE)m_EmbeddedGame, "initDll");
	*(void**)&m_FuncResize = ::GetProcAddress((HMODULE)m_EmbeddedGame, "resize");
	*(void**)&m_FuncUpdate = ::GetProcAddress((HMODULE)m_EmbeddedGame, "update");

	bool result = m_InitDll(m_Wnd, "Messiah", dllDirectory.Get(), scriptPath.Get(), "main", args.Get());
	m_FuncResize(m_Wnd);
	return result;
	return true;
}

void MessiahGame::CreateViewWindow()
{
	// Create application window
	WNDCLASSEX wc = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		DefWindowProc,
		0L,
		0L,
		GetModuleHandle(NULL),
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		NULL,
		NULL,
		L"Messiah",
		NULL
	};
	RegisterClassEx(&wc);
	m_Wnd = ::CreateWindow(L"Messiah", L"Messiah", WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, 1334, 750, (WinHandle)Application::Instance().GetWindow(), NULL, wc.hInstance, NULL);

	if (!m_Wnd)
	{
		LERROR("MessiahGame", "Create window error: %d, %p\n", Platform::GetLastErrorNo());
		return;
	}

	// Show the window
	::ShowWindow(m_Wnd, SW_SHOWDEFAULT);
	::UpdateWindow(m_Wnd);
}

// SetWindowPos
//------------------------------------------------------------------------------
void MessiahGame::SetWindowPos(int x, int y, int width, int height)
{
	if (m_Wnd)
	{
		::SendMessage(m_Wnd, WM_ENTERSIZEMOVE, 0, 0);
		::MoveWindow(m_Wnd, x, y, width, height, true);
		::SendMessage(m_Wnd, WM_EXITSIZEMOVE, 0, 0);		
	}
}

// UpdateGame
//------------------------------------------------------------------------------
void MessiahGame::UpdateGame()
{
	if (m_EmbeddedGame)
	{
		m_FuncResize(m_Wnd);
		m_FuncUpdate();
	}
}

// ShowWindow
//------------------------------------------------------------------------------
void MessiahGame::ShowWindow(bool show)
{
	if (m_Wnd)
	{
		::ShowWindow(m_Wnd, show);
	}
}


//------------------------------------------------------------------------------