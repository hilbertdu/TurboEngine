// String.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_TEMPLATE_MACROS_H
#define FOUNDATION_TEMPLATE_MACROS_H


#define MACRO_EXPAND(X)			X			// https://stackoverflow.com/questions/5134523/msvc-doesnt-expand-va-args-correctly
#define MACRO_CAT(A, B)			A##B
#define MACRO_SELECT(NAME, NUM)	MACRO_CAT(NAME##_, NUM)

#define MACRO_GET_INDEX_1(INDEX, ...) INDEX
#define MACRO_GET_INDEX_2(_1, INDEX, ...) INDEX
#define MACRO_GET_INDEX_3(_1, _2, INDEX, ...) INDEX
#define MACRO_GET_INDEX_4(_1, _2, _3, INDEX, ...) INDEX
#define MACRO_GET_INDEX_5(_1, _2, _3, _4, INDEX, ...) INDEX
#define MACRO_GET_INDEX_6(_1, _2, _3, _4, _5, INDEX, ...) INDEX
#define MACRO_GET_INDEX_7(_1, _2, _3, _4, _5, _6, INDEX, ...) INDEX
#define MACRO_GET_INDEX_8(_1, _2, _3, _4, _5, _6, _7, INDEX, ...) INDEX

#define MACRO_GET_COUNT(_1, _2, _3, _4, _5, _6, _7, _8, COUNT, ...) COUNT

#define MACRO_VA_SIZE(...)			MACRO_EXPAND(MACRO_GET_COUNT(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1))
#define MACRO_VA_INDEX(INDEX, ...)	MACRO_EXPAND(MACRO_CAT(MACRO_GET_INDEX_, INDEX)(__VA_ARGS__))
#define MACRO_VA_SELECT(NAME, ...)	MACRO_EXPAND(MACRO_SELECT(NAME, MACRO_VA_SIZE(__VA_ARGS__))(__VA_ARGS__))

#define MACRO_VA_UNPACK_2(PREFIX, _1)		PREFIX _1
#define MACRO_VA_UNPACK_3(PREFIX, _1, ...)	PREFIX _1, MACRO_EXPAND(MACRO_VA_UNPACK_2(PREFIX, __VA_ARGS__))
#define MACRO_VA_UNPACK_4(PREFIX, _1, ...)	PREFIX _1, MACRO_EXPAND(MACRO_VA_UNPACK_3(PREFIX, __VA_ARGS__))
#define MACRO_VA_UNPACK_5(PREFIX, _1, ...)	PREFIX _1, MACRO_EXPAND(MACRO_VA_UNPACK_4(PREFIX, __VA_ARGS__))
#define MACRO_VA_UNPACK_6(PREFIX, _1, ...)	PREFIX _1, MACRO_EXPAND(MACRO_VA_UNPACK_5(PREFIX, __VA_ARGS__))
#define MACRO_VA_UNPACK_7(PREFIX, _1, ...)	PREFIX _1, MACRO_EXPAND(MACRO_VA_UNPACK_6(PREFIX, __VA_ARGS__))
#define MACRO_VA_UNPACK_8(PREFIX, _1, ...)	PREFIX _1, MACRO_EXPAND(MACRO_VA_UNPACK_7(PREFIX, __VA_ARGS__))
#define MACRO_VA_UNPACK_9(PREFIX, _1, ...)	PREFIX _1, MACRO_EXPAND(MACRO_VA_UNPACK_8(PREFIX, __VA_ARGS__))

#define MACRO_VA_MAPPER(PREFIX, ...)	MACRO_EXPAND(MACRO_VA_SELECT(MACRO_VA_UNPACK, PREFIX, __VA_ARGS__))


//------------------------------------------------------------------------------
#endif // FOUNDATION_TEMPLATE_MACROS_H