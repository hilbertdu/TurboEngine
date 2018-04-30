// UIWidgets.h
//------------------------------------------------------------------------------
#pragma once
#ifndef MESSIAH_AUTOPACK_UI_BASIC_WIDGET_H
#define MESSIAH_AUTOPACK_UI_BASIC_WIDGET_H

// Includes
//------------------------------------------------------------------------------
#include "imgui/imgui.h"
#include "imgui/imgui_user.h"
#include "imgui/imgui_dock.h"
#include "Foundation/Platform/Types.h"
#include "Foundation/String/String.h"
#include "Foundation/String/StringView.h"
#include "Foundation/Math/Math.h"
#include "Foundation/Reflection/Objects/Object.h"


// class UIWidget
//------------------------------------------------------------------------------
class UIWidget : public IObject
{
public:
	UIWidget();
	UIWidget(AStringView name);
	virtual ~UIWidget();

	virtual void OnFrameUpdate() {}

	inline void SetName(AStringView name) { m_Name = name.Get(); }
	inline void SetOpened(bool opened) { m_Opened = opened; }

	inline float GetWidth() const { return m_Size.x; }
	inline float GetHeight() const { return m_Size.y; }
	inline const Vector2& GetSize() const { return m_Size; }

protected:
	AString	m_Name;
	Vector2 m_Pos;
	Vector2	m_Size;
	AString m_Parent;
	bool	m_Opened;

	TREFLECTION_DECLARE(UIWidget, IObject)
};

//------------------------------------------------------------------------------
#endif // MESSIAH_AUTOPACK_UI_BASIC_WIDGET_H