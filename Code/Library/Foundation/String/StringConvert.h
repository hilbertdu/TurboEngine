// StringConvert.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_STRING_CONVERT_H
#define FOUNDATION_STRING_CONVERT_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/String/String.h"
#include "Foundation/String/StringView.h"


namespace StringCvt
{
	// to utf-8 string
	static AString ConvertToUTF8(const AStringView & str);
}

#include "Foundation/String/StringConvert.inl"


//------------------------------------------------------------------------------
#endif // FOUNDATION_STRING_CONVERT_H