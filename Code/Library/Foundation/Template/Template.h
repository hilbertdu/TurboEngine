// Template.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_TEMPLATE_STRING_H
#define FOUNDATION_TEMPLATE_STRING_H

// Includes
//------------------------------------------------------------------------------
#include <tuple>

// String
//------------------------------------------------------------------------------
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


// Selector
//------------------------------------------------------------------------------
template<uint32 N, typename... T>
using Selector = typename std::tuple_element<N, std::tuple<T...>>::type;


// Function traits
//------------------------------------------------------------------------------
template<class F>
struct FunctionTraits;

// Function pointer
template<class R, class... Args>
struct FunctionTraits<R(*)(Args...)> : public FunctionTraits<R(Args...)>
{};

template<class R, class... Args>
struct FunctionTraits<R(Args...)>
{
	using ReturnType = R;
	using ArgsType = std::tuple<Args...>;

	static constexpr SIZET arity = sizeof...(Args);

	template <SIZET N>
	struct Argument
	{
		static_assert(N < arity, "error: invalid parameter index.");
		using Type = typename std::tuple_element<N, std::tuple<Args...>>::type;
	};
};

//------------------------------------------------------------------------------
#endif // FOUNDATION_TEMPLATE_STRING_H