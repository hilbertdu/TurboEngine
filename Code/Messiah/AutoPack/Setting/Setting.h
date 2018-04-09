// Setting.h
//------------------------------------------------------------------------------
#pragma once
#ifndef MESSIAH_AUTOPACK_SETTING_H
#define MESSIAH_AUTOPACK_SETTING_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/String/String.h"
#include "Foundation/String/StringView.h"


// class UISystem
//------------------------------------------------------------------------------
class Setting
{
public:
	Setting();
	~Setting();

	inline AStringView GetUILayoutFile() const { return m_UILayoutFile; }
	inline AStringView GetServiceFile()  const { return m_ServiceFile; }

private:
	AString m_UILayoutFile;
	AString m_ServiceFile;
};

//------------------------------------------------------------------------------
#endif // MESSIAH_AUTOPACK_SETTING_H