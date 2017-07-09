// String.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_TEMPLATE_STRING_H
#define FOUNDATION_TEMPLATE_STRING_H

// char sequence for generate compiler string
template<char... CHAR>
struct Sequence
{
	static constexpr char Str[sizeof...(CHAR) + 1] = { CHAR..., '\0' };
};

#define MACRO_GET_STR_1(STR, INDEX)		(sizeof(STR) > (INDEX) ? STR[(INDEX)] : 0)
#define MACRO_GET_STR_4(STR, INDEX)		MACRO_GET_STR_1(STR, INDEX), MACRO_GET_STR_1(STR, INDEX + 1), MACRO_GET_STR_1(STR, INDEX + 2), MACRO_GET_STR_1(STR, INDEX + 3)
#define MACRO_GET_STR_16(STR, INDEX)	MACRO_GET_STR_4(STR, INDEX), MACRO_GET_STR_4(STR, INDEX + 4), MACRO_GET_STR_4(STR, INDEX + 8), MACRO_GET_STR_4(STR, INDEX + 12)
#define MACRO_GET_STR_64(STR, INDEX)	MACRO_GET_STR_16(STR, INDEX), MACRO_GET_STR_16(STR, INDEX + 16), MACRO_GET_STR_16(STR, INDEX + 32), MACRO_GET_STR_16(STR, INDEX + 48)

#define MACRO_GET_STR(STR)	MACRO_GET_STR_64(STR, 0), 0		//guard for longer strings
#define CONSTSTR(STR)		Sequence<MACRO_GET_STR(STR)>

//------------------------------------------------------------------------------
#endif // FOUNDATION_TEMPLATE_STRING_H