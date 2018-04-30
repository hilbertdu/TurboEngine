// MessiahGame.h
//------------------------------------------------------------------------------
#pragma once
#ifndef MESSIAH_AUTOPACK_EXTERNAL_MESSIAHGAME_H
#define MESSIAH_AUTOPACK_EXTERNAL_MESSIAHGAME_H

// Includes
//------------------------------------------------------------------------------
#include <windows.h>

// MessiahGame
//------------------------------------------------------------------------------
class MessiahGame
{
public:
	using WinHandle = HWND;
	using Module = HMODULE;
	typedef bool(*FuncInitDll)(void*, const char*, const char*, const char*, const char*, const char*);
	typedef bool(*FuncResize)(void*);
	typedef void(*FuncUpdate)();

	MessiahGame() : m_EmbeddedGame(nullptr) {}
	~MessiahGame() {}

	bool LoadEmbeddedGame();
	void SetWindowPos(int x, int y, int width, int height);
	void UpdateGame();
	void ShowWindow(bool show);

private:
	void CreateViewWindow();

	Module m_EmbeddedGame;
	WinHandle m_Wnd;

	FuncInitDll m_InitDll;
	FuncResize	m_FuncResize;
	FuncUpdate	m_FuncUpdate;
};


//------------------------------------------------------------------------------
#endif // MESSIAH_AUTOPACK_EXTERNAL_MESSIAHGAME_H