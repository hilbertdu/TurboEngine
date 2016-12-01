#include "Array.h"
//#include <iostream>
#include "Foundation/Platform/Misc.h"
#include "Foundation/String/String.h"

Array<int> a;
StackArray<int, 200> b;

AString str1;
AStackString<1024> str2("1234");

#define ONE 1

#define C_1(c) #c

#define AA(a) A(a)

#define A(a) \
	do {								\
		PRAGMA_DISABLE_PUSH_MSVC(4127)	\
		std::cout << C_##a(100) << std::endl;	\
	} while (0);						\
	PRAGMA_DISABLE_POP_MSVC

#define B_1(b1) A(b1)
#define BB(b) _BBX(b)
#define _BBX(b) B_##b(1)

void main()
{
	wchar_t c = ' ';
	//a.Append(100);
	//BB(ONE)
	str1.GetLength();
}

