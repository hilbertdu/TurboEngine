// Application.h
//------------------------------------------------------------------------------
#pragma once
#ifndef MESSIAH_AUTOPACK_APPLICATION_H
#define MESSIAH_AUTOPACK_APPLICATION_H

// Include
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"
#include "Foundation/Platform/Misc.h"
#include "Foundation/Memory/Allocator.h"
#include "Foundation/Memory/Mem.h"
#include "Foundation/Math/Conversion.h"
#include <type_traits>


class Application
{
public:
	void CreateWindow(const char * name);
	void InitImGui();
	void ImGuiFrameStart();
	void ImGuiShutdown();
};


//------------------------------------------------------------------------------
#endif // MESSIAH_AUTOPACK_APPLICATION_H
