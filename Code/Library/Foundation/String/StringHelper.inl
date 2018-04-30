

namespace StringHelper
{
	template<typename CharType>
	void ToLower(CharType * src, SIZET size)
	{
		CharType * pos = src;
		CharType * end = src + size;
		while (pos < end)
		{
			CharType c = *pos;
			if ((c >= 'A') && (c <= 'Z'))
			{
				c = 'a' + (c - 'A');
				*pos = c;
			}
			pos++;
		}
	}

	template<typename CharType>
	void ToUpper(CharType * src, SIZET size)
	{
		CharType * pos = src;
		CharType * end = src + size;
		while (pos < end)
		{
			CharType c = *pos;
			if ((c >= 'a') && (c <= 'z'))
			{
				c = 'A' + (c - 'a');
				*pos = c;
			}
			pos++;
		}
	}

	template<typename CharType>
	const CharType * Find(const CharType * src, SIZET size, CharType c, const CharType * startPos)
	{
		// If startPos is provided, validate it
		// (deliberately allow startPos to point one past end of string)
		ASSERT((startPos == nullptr) || (startPos >= src));
		ASSERT((startPos == nullptr) || (startPos <= src + size));

		const CharType * pos = startPos ? startPos : src;
		const CharType * end = src + size;
		while (pos < end)
		{
			if (*pos == c)
			{
				return pos;
			}
			pos++;
		}
		return nullptr;
	}

	template<typename CharType>
	const CharType * Find(const CharType * src, SIZET size, const CharType * subString)
	{
		SIZET subStrLen = StrLen(subString);

		const CharType * pos = m_Contents;
		const CharType * end = pos + m_Length - subStrLen;
		while (pos <= end)
		{
			if (StrNCmp(pos, subString, subStrLen) == 0)
			{
				return pos;
			}
			pos++;
		}
		return nullptr;
	}

	template<typename CharType>
	const CharType * FindI(const CharType * src, SIZET size, const CharType * subString)
	{
		SIZET subStrLen = StrLen(subString);

		const CharType * pos = src;
		const CharType * end = pos + size - subStrLen;
		while (pos <= end)
		{
			if (StrNCmpI(pos, subString, subStrLen) == 0)
			{
				return pos;
			}
			pos++;
		}
		return nullptr;
	}

	template<typename CharType>
	const CharType * FindLast(const CharType * src, SIZET size, CharType c)
	{
		const CharType * pos = src + size - 1;
		while (pos >= src)
		{
			if (*pos == c)
			{
				return pos;
			}
			pos--;
		}
		return nullptr;
	}

	template<typename CharType>
	bool EndsWith(const CharType * src, SIZET size, CharType c)
	{
		SIZET len = size;
		if (len == 0)
		{
			return false;
		}
		return (src[len - 1] == c);
	}

	template<typename CharType>
	bool EndsWith(const CharType * src, SIZET size, const CharType * string)
	{
		const SIZET stringLen = StrLen(string);
		const CharType * possiblePos = src + size - stringLen;
		if (possiblePos < src)
		{
			return false; // String to search is longer than this string
		}
		return (StrNCmp(possiblePos, string, stringLen) == 0);
	}

	template<typename CharType>
	bool EndsWithI(const CharType * src, SIZET size, const CharType * other)
	{
		const SIZET otherLen = StrLen(other);
		if (otherLen > size)
		{
			return false;
		}
		return (StrNCmpI(src + size - otherLen, other, otherLen) == 0);
	}

	template<typename CharType>
	bool BeginsWith(const CharType * src, CharType c)
	{
		if (src == nullptr)
		{
			return false;
		}
		return (src[0] == c);
	}

	template<typename CharType>
	bool BeginsWith(const CharType * src, const CharType * string)
	{
		SIZET otherLen = StrLen(string);
		if (otherLen > StrLen(src))
		{
			return false;
		}
		return (StrNCmp(src, string, otherLen) == 0);
	}

	template<typename CharType>
	bool BeginsWithI(const CharType * src, const CharType * string)
	{
		SIZET otherLen = StrLen(string);
		if (otherLen > StrLen(src))
		{
			return false;
		}
		return (StrNCmpI(src, string, otherLen) == 0);
	}

	// Match
	// NOTE: This code is based on that in the C/C++ Users Journal (Mike Cornelison)
	//------------------------------------------------------------------------------
	template<typename CharType>
	bool Match(const CharType * pat, const CharType * str)
	{
		bool star;

	new_segment:
		star = false;
		if (*pat == '*')
		{
			star = true;
			do { pat++; } while (*pat == '*');
		}

	test_match:
		int i;
		for (i = 0; pat[i] && (pat[i] != '*'); i++)
		{
			CharType a = str[i];
			CharType b = pat[i];
			if (a != b)
			{
				if (!str[i]) return false;
				if ((pat[i] == '?') && (str[i] != '.')) continue;
				if (!star) return false;
				str++;
				goto test_match;
			}
		}
		if (pat[i] == '*')
		{
			str += i;
			pat += i;
			goto new_segment;
		}
		if (!str[i]) return true;
		if (i && pat[i - 1] == '*') return true;
		if (!star) return false;
		str++;
		goto test_match;
	}

	// MatchI
	// NOTE: This code is based on that in the C/C++ Users Journal (Mike Cornelison)
	//------------------------------------------------------------------------------
	template<typename CharType>
	bool MatchI(const CharType * pat, const CharType * str)
	{
		bool star;

	new_segment:
		star = false;
		if (*pat == '*')
		{
			star = true;
			do { pat++; } while (*pat == '*');
		}

	test_match:
		int i;
		for (i = 0; pat[i] && (pat[i] != '*'); i++)
		{
			CharType a = str[i]; a = ((a >= 'A') && (a <= 'Z')) ? 'a' + (a - 'A') : a;
			CharType b = pat[i]; b = ((b >= 'A') && (b <= 'Z')) ? 'a' + (b - 'A') : b;
			if (a != b)
			{
				if (!str[i]) return false;
				if ((pat[i] == '?') && (str[i] != '.')) continue;
				if (!star) return false;
				str++;
				goto test_match;
			}
		}
		if (pat[i] == '*')
		{
			str += i;
			pat += i;
			goto new_segment;
		}
		if (!str[i]) return true;
		if (i && pat[i - 1] == '*') return true;
		if (!star) return false;
		str++;
		goto test_match;
	}

	template<class CharType>
	bool Matches(const CharType * src, SIZET size, const CharType * pattern)
	{
		return Match(src, size, pattern, src);
	}

	template<class CharType>
	bool MatchesI(const CharType * src, SIZET size, const CharType * pattern)
	{
		return MatchI(src, size, pattern, src);
	}

	template<typename CharType>
	void Copy(const CharType * src, CharType * dst, SIZET len)
	{
		if (len == 0)
		{
			return;
		}
		const CharType * end = src + len;
		while (src < end)
		{
			*dst++ = *src++;
		}
		*dst = (CharType)'\000';
	}

	template<typename CharType>
	SIZET StrLen(const CharType * string)
	{
		const CharType * pos = string;
		while (*pos != (CharType)'\000')
		{
			pos++;
		}
		return (pos - string);
	}

	template<typename CharType>
	uint32 StrNCmp(const CharType * a, const CharType * b, SIZET num)
	{
		while (num > 0)
		{
			if (*a == *b)
			{
				if (*a == (CharType)'\000')
				{
					return 0; // Both strings ended and equal
				}

				// Keep going
				a++;
				b++;
				num--;
				continue;
			}

			// Different, so return relationship
			return (*a > *b) ? 1 : -1;
		};
		return 0; // Strings identical upto 'num' CharTypes
	}

	template<typename CharType>
	uint32 StrNCmpI(const CharType * a, const CharType * b, SIZET num)
	{
		while (num > 0)
		{
			CharType a1 = *a;
			if ((a1 >= 'A') && (a1 <= 'Z'))
			{
				a1 = 'a' + (a1 - 'A');
			}
			CharType b1 = *b;
			if ((b1 >= 'A') && (b1 <= 'Z'))
			{
				b1 = 'a' + (b1 - 'A');
			}
			if (a1 == b1)
			{
				if (a1 == (CharType)'\000')
				{
					return 0; // Both strings ended and equal
				}

				// Keep going
				a++;
				b++;
				num--;
				continue;
			}

			// Different, so return relationship
			return (a1 - b1);
		};
		return 0; // Strings identical upto 'num' CharTypes
	}

	// Compare
	template<class CharType>
	bool Compare(const CharType * src, const CharType * des)
	{
		return strcmp(src, des) == 0;
	}

	int32 CompareI(const CHAR * src, const CHAR * des)
	{
	#if defined(__WINDOWS__)
		return _stricmp(src, des);
	#elif defined(__APPLE__)
		return strcasecmp(src, des);
	#else
		#error Unknown platform
	#endif
	}

	int32 CompareI(const WCHAR * src, const WCHAR * des)
	{
		return 0;
	}

	template<class CharType, class StringType>
	void Tokenize(const CharType * src, SIZET size, Array<StringType> & tokens, CharType splitChar)
	{
		Array<const CharType *> tokenStarts;
		Array<const CharType *> tokenEnds;

		const CharType * pos = src;
		const CharType * end = src + size;
		bool lookingForStart = true;
		CharType quoteChar = 0;
		while (pos < end)
		{
			if (lookingForStart)
			{
				if (*pos == splitChar)
				{
					++pos;
					continue;
				}

				// found the start of a new token
				tokenStarts.Append(pos);
				lookingForStart = false;
			}

			// hit a quote?
			CharType c = *pos;
			if ((c == '"') || (c == '\''))
			{
				if (quoteChar == 0)
				{
					// opening quote
					quoteChar = c;
				}
				else if (quoteChar == c)
				{
					// closing quote
					quoteChar = 0;
				}
				else
				{
					// quote of the 'other' type - consider as part of token
				}
			}
			else if (c == splitChar)
			{
				if (quoteChar == 0)
				{
					tokenEnds.Append(pos);
					lookingForStart = true;
				}
				else
				{
					// space inside quoted token - consider as part of token
				}
			}
			else
			{
				// normal CharTypeacter part of token
			}
			++pos;
		}
		ASSERT((tokenStarts.GetSize() == tokenEnds.GetSize()) ||
			(tokenStarts.GetSize() == (tokenEnds.GetSize() + 1)));
		if (tokenStarts.GetSize() > tokenEnds.GetSize())
		{
			tokenEnds.Append(pos);
		}
		ASSERT(tokenStarts.GetSize() == tokenEnds.GetSize());

		// pre-size output to avoid reallocations
		tokens.Clear();
		const SIZET numTokens(tokenStarts.GetSize());
		if (tokens.GetCapacity() < numTokens)
		{
			tokens.SetCapacity(numTokens);
		}
		tokens.SetSize(numTokens);

		// copy tokens
		for (SIZET i = 0; i < numTokens; ++i)
		{
			tokens[i].Assign(tokenStarts[i], tokenEnds[i]);
		}
	}

	// Format
	template<class StringType>
	void Format(StringType & des, const CHAR * fmtString, ...)
	{
		va_list args;
		va_start(args, fmtString);
		VFormat(des, fmtString, args);
		va_end(args);
	}

	template<class StringType>
	void Format(StringType & des, const WCHAR * fmtString, ...)
	{
	}

	template<class StringType>
	void VFormat(StringType & des, const CHAR * fmtString, va_list arg)
	{
		// Try to work entirely on the stack
		const int32 STACK_BUFFER_SIZE(8 * KILOBYTE);
		CHAR stackBuffer[STACK_BUFFER_SIZE]{ 0 };
		CHAR * buffer = stackBuffer;
		size_t bufferSize = STACK_BUFFER_SIZE;

		// Attempt the formatting
		int len = 0;
		while (true)
		{
		#if defined(__WINDOWS__)
			len = vsnprintf_s(buffer, bufferSize, _TRUNCATE, fmtString, arg);
		#elif defined(__APPLE__)
			len = vsnprintf(buffer, bufferSize, fmtString, args);
		#else
			#error Unknown platform
		#endif
			if (len >= 0)
			{
				break;
			}
			else
			{
				if (buffer != stackBuffer)
				{
					FREE(buffer);
				}
				bufferSize *= 2;
				buffer = (CHAR *)ALLOC(bufferSize);
			}
		}

		// Keep the final result
		des.Assign(buffer, buffer + len);

		// Free memory if we allocated any
		if (buffer != stackBuffer)
		{
			FREE(buffer);
		}
	}

	template<class StringType>
	void VFormat(StringType & des, const WCHAR * fmtString, va_list arg)
	{
	}
}