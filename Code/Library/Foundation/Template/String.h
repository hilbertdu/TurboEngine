// String.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_TEMPLATE_STRING_H
#define FOUNDATION_TEMPLATE_STRING_H

// char sequence for generate compiler string
template<char... c>
struct Sequence
{
	static constexpr char Str[sizeof...(c)+1] = { c..., '\0' };
};


#define MACRO_GET_1(str, i) \
    (sizeof(str) > (i) ? str[(i)] : 0)

#define MACRO_GET_4(str, i) \
    MACRO_GET_1(str, i+0),  \
    MACRO_GET_1(str, i+1),  \
    MACRO_GET_1(str, i+2),  \
    MACRO_GET_1(str, i+3)

#define MACRO_GET_16(str, i) \
    MACRO_GET_4(str, i+0),   \
    MACRO_GET_4(str, i+4),   \
    MACRO_GET_4(str, i+8),   \
    MACRO_GET_4(str, i+12)

#define MACRO_GET_64(str, i) \
    MACRO_GET_16(str, i+0),  \
    MACRO_GET_16(str, i+16), \
    MACRO_GET_16(str, i+32), \
    MACRO_GET_16(str, i+48)

#define MACRO_GET_STR(str) MACRO_GET_64(str, 0), 0 //guard for longer strings
#define CONSTSTR(str) Sequence<MACRO_GET_STR(str)>


template <int N, class... T>
struct ParamIndex;

template <typename T0, typename... T>
struct ParamIndex<0, T0, T...> {
	using Type = void;
};

template <typename T0, typename... T>
struct ParamIndex<1, T0, T...> {
	using Type = T0;
};

template <int N, typename T0, typename... T>
struct ParamIndex<N, T0, T...> {
	using Type = typename ParamIndex<N - 1, T...>::Type;
};

//------------------------------------------------------------------------------
#endif // FOUNDATION_TEMPLATE_STRING_H