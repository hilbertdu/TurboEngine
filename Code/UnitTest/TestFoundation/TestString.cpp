// TestString.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "TestFramework/UnitTest.h"

#include "Foundation/String/String.h"
#include "Foundation/Container/SmartPtr.h"

#include <memory.h>

// TestString
//------------------------------------------------------------------------------
class TestString : public UnitTest
{
private:
	DECLARE_TESTS

	void AStringConstructors() const;
	void AStringAssignment() const;
	void AStackStringConstructors() const;
	void AStackStringOverflow() const;
	void BigString() const;
	void EndsWithI() const;
	void Find() const;
	void Tokenize() const;
	void PatternMatch() const;
	void PatternMatchI() const;
	void StringMove() const;
};

// Register Tests
//------------------------------------------------------------------------------
REGISTER_TESTS_BEGIN(TestString)
	REGISTER_TEST(AStringConstructors)
	REGISTER_TEST(AStringAssignment)
	REGISTER_TEST(AStackStringConstructors)
	REGISTER_TEST(AStackStringOverflow)
	REGISTER_TEST(BigString)
	REGISTER_TEST(EndsWithI)
	REGISTER_TEST(Find)
	REGISTER_TEST(Tokenize)
	REGISTER_TEST(PatternMatch)
	REGISTER_TEST(PatternMatchI)
	REGISTER_TEST(StringMove)
REGISTER_TESTS_END

// AStringConstructors
//------------------------------------------------------------------------------
void TestString::AStringConstructors() const
{
	{
		// AString with no arguments
		AString empty;
		TEST_ASSERT(empty.GetLength() == 0);
		TEST_ASSERT(empty.IsEmpty() == true);
	}
	{
		// AString with reserve capacity argument
		AString empty;
		empty.SetCapacity(16);
		TEST_ASSERT(empty.GetLength() == 0);
		TEST_ASSERT(empty.GetCapacity() == 16);
		TEST_ASSERT(empty.IsEmpty() == true);
	}
	{
		// AString from char *
		AString fromCharStar("hello");
		TEST_ASSERT(fromCharStar.GetLength() == 5);
		TEST_ASSERT(fromCharStar.IsEmpty() == false);

		// AString from AString
		AString fromAString(fromCharStar);
		TEST_ASSERT(fromAString.GetLength() == 5);
		TEST_ASSERT(fromAString.IsEmpty() == false);
	}
	{
		const char * hello = "hellohellohello";
		AString fromCharStarPair(hello, hello + 5);
		TEST_ASSERT(fromCharStarPair.GetLength() == 5);
		TEST_ASSERT(fromCharStarPair.IsEmpty() == false);
	}
}

// AStringAssignment
//------------------------------------------------------------------------------
void TestString::AStringAssignment() const
{
	AString str;
	str = "test";
	TEST_ASSERT(str.GetLength() == 4);
	TEST_ASSERT(str.IsEmpty() == false);

	AString str2;
	str2 = str;
	TEST_ASSERT(str2.GetLength() == 4);
	TEST_ASSERT(str2.IsEmpty() == false);

	const char * testData = "hellozzzzzzzzz";
	AString str3;
	str3.Assign(testData, testData + 5);
	TEST_ASSERT(str3.GetLength() == 5);
	TEST_ASSERT(str3.IsEmpty() == false);

	{
		AString dst;
		dst.Assign(AString::GetEmpty());
	}
	{
		AString dst;
		dst.Assign(AString::GetEmpty().Get(), AString::GetEmpty().Get());
	}
	{
		str3 += " ";
		str3 += "world";
		TEST_ASSERT(str3 == "hello world");
	}
	{
		str3.Append("!!!!!", 1);
		TEST_ASSERT(str3 == "hello world!");
	}
	{
		str3.Append(str);
		TEST_ASSERT(str3 == "hello world!test");
	}
}

// AStackStringConstructors
//------------------------------------------------------------------------------
void TestString::AStackStringConstructors() const
{
	{
		// AStackString<> with no arguments
		AStackString<> empty;
		TEST_ASSERT(empty.GetLength() == 0);
		TEST_ASSERT(empty.GetCapacity() == 0);
		TEST_ASSERT(empty.IsEmpty() == true);
	}
	{
		// AStackString<> from char *
		AStackString<> fromCharStar("hello");
		TEST_ASSERT(fromCharStar.GetLength() == 5);
		TEST_ASSERT(fromCharStar.GetCapacity() >= 5);
		TEST_ASSERT(fromCharStar.IsEmpty() == false);

		// AStackString<> from AStackString
		AStackString<> fromAString(fromCharStar);
		TEST_ASSERT(fromAString.GetLength() == 5);
		TEST_ASSERT(fromAString.GetCapacity() >= 5);
		TEST_ASSERT(fromAString.IsEmpty() == false);
	}
	{
		// AStackString<> from AString
		AString astring("hello");
		AStackString<> fromAString(astring);
		TEST_ASSERT(fromAString.GetLength() == 5);
		TEST_ASSERT(fromAString.GetCapacity() >= 5);
		TEST_ASSERT(fromAString.IsEmpty() == false);
	}
	{
		const char * hello = "hellohellohello";
		AStackString<32> fromCharStarPair(hello, hello + 5);
		TEST_ASSERT(fromCharStarPair.GetLength() == 5);
		TEST_ASSERT(fromCharStarPair.GetCapacity() >= 5);
		TEST_ASSERT(fromCharStarPair.IsEmpty() == false);
	}
}

// AStackStringOverflow
//------------------------------------------------------------------------------
void TestString::AStackStringOverflow() const
{
	{
		// constructor with string longer than buffer
		AStackString<8, false> string("0123456");
		TEST_ASSERT(string.GetLength() == 7);
		TEST_ASSERT(string.GetLength() == AString::StrLen(string.Get()));
	}
	{
		// assigned of string longer than buffer
		AStackString<8> string;
		string = "01234567890123456789";
		TEST_ASSERT(string.GetLength() == 20);
		TEST_ASSERT(string.GetLength() == AString::StrLen(string.Get()));
	}
	{
		// concetentation of string longer than buffer
		AStackString<8> string;
		string += "01234567890123456789";
		TEST_ASSERT(string.GetLength() == 20);
		TEST_ASSERT(string.GetLength() == AString::StrLen(string.Get()));
	}
}

// BigString
//------------------------------------------------------------------------------
void TestString::BigString() const
{
	// create a massive string
	AutoPtr<char, AllocDeletor> mem((char *)ALLOC((10 * MEGABYTE) + 1));
	MemSet(mem.Get(), 'a', 10 * MEGABYTE);
	mem.Get()[10 * MEGABYTE] = '\000';

	// create a stack string
	AStackString<2048> string;

	// now overflow massively so the buffer resizes
	// (string will now be 10MB, which is well over the 64K limit
	// previously imposed on strings)
	string += mem.Get();
	TEST_ASSERT(string.GetLength() == 10 * MEGABYTE);
	TEST_ASSERT(string.GetLength() == AString::StrLen(string.Get()));
}

// EndsWithI
//------------------------------------------------------------------------------
void TestString::EndsWithI() const
{
	// empty string is handled
	{
		AString empty;
		TEST_ASSERT(empty.EndsWithI(AString("hello")) == false);
		TEST_ASSERT(empty.EndsWithI("hello") == false);
	}

	// empty compare string is handled
	{
		AString empty;
		TEST_ASSERT(empty.EndsWithI(AString::GetEmpty()) == true);
		TEST_ASSERT(empty.EndsWithI("") == true);
	}

	// compare with longer string is handled
	{
		AString shortString("short");
		TEST_ASSERT(shortString.EndsWithI(AString("Does not end with this")) == false);
		TEST_ASSERT(shortString.EndsWithI("Does not end with this") == false);
	}

	// compare with shorter string is handled
	{
		AString str("this is a string ending with Chicken");
		TEST_ASSERT(str.EndsWithI("Chicken") == true);
		TEST_ASSERT(str.EndsWithI("chicken") == true);
		TEST_ASSERT(str.EndsWithI("ICKEN") == true);
		TEST_ASSERT(str.EndsWithI("Chicken ") == false);
		TEST_ASSERT(str.EndsWithI("Turkey") == false);
	}
}

// Find
//------------------------------------------------------------------------------
void TestString::Find() const
{
	// BUG: Returning contents past end of string
	{
		AString tmp("12345678--X---");
		TEST_ASSERT(tmp.Find('X')); // This was OK
		TEST_ASSERT(tmp.Find('X', tmp.Get() + 8)); // This was returning junk data past end of string
	}
}

// Tokenize
//------------------------------------------------------------------------------
void TestString::Tokenize() const
{
	// empty
	{
		Array<AString> tokens;
		AString::GetEmpty().Tokenize(tokens);
		TEST_ASSERT(tokens.GetSize() == 0);
	}
	{
		Array<AString> tests;
		tests.SetCapacity(1);
		tests.SetSize(1);
	}

	// one token
	{
		Array<AString> tokens;
		AStackString<> input("one_token");
		input.Tokenize(tokens);
		TEST_ASSERT(tokens.GetSize() == 1);
		TEST_ASSERT(tokens[0] == "one_token");
	}

	// multiple tokens
	{
		Array<AString> tokens;
		AStackString<> input("this is four tokens");
		input.Tokenize(tokens);
		TEST_ASSERT(tokens.GetSize() == 4);
		TEST_ASSERT(tokens[0] == "this");
		TEST_ASSERT(tokens[1] == "is");
		TEST_ASSERT(tokens[2] == "four");
		TEST_ASSERT(tokens[3] == "tokens");
	}

	// leading whitespace
	{
		Array<AString> tokens;
		AStackString<> input("     token");
		input.Tokenize(tokens);
		TEST_ASSERT(tokens.GetSize() == 1);
		TEST_ASSERT(tokens[0] == "token");
	}

	// trailing whitespace
	{
		Array<AString> tokens;
		AStackString<> input("token      ");
		input.Tokenize(tokens);
		TEST_ASSERT(tokens.GetSize() == 1);
		TEST_ASSERT(tokens[0] == "token");
	}

	// multiple whitespaces
	{
		Array<AString> tokens;
		AStackString<> input("   lots  of      spaces   ");
		input.Tokenize(tokens);
		TEST_ASSERT(tokens.GetSize() == 3);
		TEST_ASSERT(tokens[0] == "lots");
		TEST_ASSERT(tokens[1] == "of");
		TEST_ASSERT(tokens[2] == "spaces");
	}

	// single quotes
	{
		Array<AString> tokens;
		AStackString<> input("this is 'only three tokens'");
		input.Tokenize(tokens);
		TEST_ASSERT(tokens.GetSize() == 3);
		TEST_ASSERT(tokens[0] == "this");
		TEST_ASSERT(tokens[1] == "is");
		TEST_ASSERT(tokens[2] == "'only three tokens'");
	}

	// double quotes
	{
		Array<AString> tokens;
		AStackString<> input("this is \"only three tokens\"");
		input.Tokenize(tokens);
		TEST_ASSERT(tokens.GetSize() == 3);
		TEST_ASSERT(tokens[0] == "this");
		TEST_ASSERT(tokens[1] == "is");
		TEST_ASSERT(tokens[2] == "\"only three tokens\"");
	}

	// mixed quotes
	{
		Array<AString> tokens;
		AStackString<> input("'on token with an \" in it'");
		input.Tokenize(tokens);
		TEST_ASSERT(tokens.GetSize() == 1);
		TEST_ASSERT(tokens[0] == "'on token with an \" in it'");
	}

	// alternate split char
	{
		Array<AString> tokens;
		AStackString<> input("c:\\path\\path;d:\\path;e:\\");
		input.Tokenize(tokens, ';');
		TEST_ASSERT(tokens.GetSize() == 3);
		TEST_ASSERT(tokens[0] == "c:\\path\\path");
		TEST_ASSERT(tokens[1] == "d:\\path");
		TEST_ASSERT(tokens[2] == "e:\\");
	}
}

// PatternMatch
//------------------------------------------------------------------------------
void TestString::PatternMatch() const
{
#define CHECK_MATCH(pat, str, match)				\
	{												\
		AStackString<> string(str);					\
		TEST_ASSERT(string.Matches(pat) == match);	\
	}

	CHECK_MATCH("*.cpp", "File.cpp", true)
	CHECK_MATCH("*", "File.cpp", true)
	CHECK_MATCH("File*.*", "File.cpp", true)
	CHECK_MATCH("*.c*", "File.cpp", true)
	CHECK_MATCH("File.cpp", "File.cpp", true)

	CHECK_MATCH("*.cpp", "File.CPP", false)
	CHECK_MATCH("File*.*", "FILE.cpp", false)
	CHECK_MATCH("*.c*", "File.CPP", false)
	CHECK_MATCH("File.cpp", "file.cpp", false)

	CHECK_MATCH("*.cpp", "File.c", false);
	CHECK_MATCH("*.cpp", "File.cpp~", false);
	CHECK_MATCH("*.cpp", "File.cpp.notcpp", false);
	CHECK_MATCH("*.cpp", "", false);

	CHECK_MATCH("*.cpp", "File.c", false);
	CHECK_MATCH("*.cpp", "File.cpp~", false);
	CHECK_MATCH("*.cpp", "File.cpp.notcpp", false);
	CHECK_MATCH("*.cpp", "", false);

#undef CHECK_MATCH
}

// PatternMatchI
//------------------------------------------------------------------------------
void TestString::PatternMatchI() const
{
#define CHECK_MATCH(pat, str, match)				\
	{												\
		AStackString<> string(str);					\
		TEST_ASSERT(string.MatchesI(pat) == match);	\
	}

	CHECK_MATCH("*.cpp", "File.cpp", true)
	CHECK_MATCH("*", "File.cpp", true)
	CHECK_MATCH("File*.*", "File.cpp", true)
	CHECK_MATCH("*.c*", "File.cpp", true)
	CHECK_MATCH("File.cpp", "File.cpp", true)

	CHECK_MATCH("*.cpp", "File.CPP", true)
	CHECK_MATCH("File*.*", "FILE.cpp", true)
	CHECK_MATCH("*.c*", "File.CPP", true)
	CHECK_MATCH("File.cpp", "file.cpp", true)

	CHECK_MATCH("*.cpp", "File.c", false);
	CHECK_MATCH("*.cpp", "File.cpp~", false);
	CHECK_MATCH("*.cpp", "File.cpp.notcpp", false);
	CHECK_MATCH("*.cpp", "", false);

#undef CHECK_MATCH
}


void TestString::StringMove() const
{
	{
		AString a1 = AString("123");
		a1 += "456";
		AString a2 = std::move(a1);
		TEST_ASSERT(a2 == "123456");
	}
	{
		AStackString<2> a1 = AStackString<2>("12");
		AStackString<2> a2 = std::move(a1);
		a2 += "34";
		AStackString<2> a3 = std::move(a2);
		TEST_ASSERT(a3 == "1234");
		AString a4 = a2;
		TEST_ASSERT(a4 == "1234");
		a4 += "56";
		a3 = a4;
		TEST_ASSERT(a3 == "123456");

		AString a5 = std::move(a2);
		TEST_ASSERT(a5 == "1234");
	}
	{
		AString a1("123456");
		AString a2 = std::move(a1);
	}
}

//------------------------------------------------------------------------------
