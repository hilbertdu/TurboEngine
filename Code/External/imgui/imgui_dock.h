#pragma once

#include "imgui.h"

namespace ImGui
{
	IMGUI_API void ShutdownDock();
	IMGUI_API void RootDock(const ImVec2& pos, const ImVec2& size);
	IMGUI_API bool BeginDock(const char* label, bool* opened = nullptr, ImGuiWindowFlags extra_flags = 0, const ImVec2& default_size = ImVec2(-1, -1));
	IMGUI_API void ForceDock(const char* label, const char* docker = nullptr, int slot_idx = 0);
	IMGUI_API void EndDock();
	IMGUI_API void SetDockActive();	
	IMGUI_API void LoadDock(int count);
	IMGUI_API void LoadDock(int index, const IMDocker& docker);
	IMGUI_API int  GetDockSize();
	IMGUI_API void FindDock(int idx, IMDocker& docker);
} // namespace ImGui

//-----------------------------------------------------------------------------