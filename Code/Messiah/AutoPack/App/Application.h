// Application.h
//------------------------------------------------------------------------------
#pragma once
#ifndef MESSIAH_AUTOPACK_APPLICATION_H
#define MESSIAH_AUTOPACK_APPLICATION_H

// Include
//------------------------------------------------------------------------------
#include "Foundation/String/String.h"
#include "Foundation/Platform/Misc.h"
#include "Foundation/Memory/Allocator.h"
#include "Foundation/Memory/Mem.h"
#include "Foundation/Math/Conversion.h"
#include "Foundation/Pattern/Singleton.h"
#include <type_traits>
#include <d3d11.h>


class Application : public Singleton<Application>
{
public:
	static void Initialize();
	static void Finalize();

	virtual void Run() = 0;
	virtual void *GetWindow() = 0;

protected:
	WStackString<64> m_AppName;
};


class ApplicationWin32 : public Application
{
public:
	using WinHandle = HWND;

	ApplicationWin32();
	virtual ~ApplicationWin32();

	virtual void Run();
	virtual void *GetWindow() { return m_HWnd; }

private:
	void CreateMainWindow();
	void DestroyWindow();

	void CreateGuiSystem();
	void DestroyGuiSystem();

	WinHandle m_HWnd;
	//Delegate<LRESULT(HWND, UINT, WPARAM, LPARAM)> m_WinProc;
};


//------------------------------------------------------------------------------
#endif // MESSIAH_AUTOPACK_APPLICATION_H
