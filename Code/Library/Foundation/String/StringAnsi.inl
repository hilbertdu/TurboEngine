// StringAnsi.inl
//-----------------------------------------------------------------------------


// String specialization
//-----------------------------------------------------------------------------
template<typename CharType, typename Allocator>
template<typename OtherAllocator, class>
int32 String<CharType, Allocator>::CompareI(const String<CHAR, OtherAllocator> & other) const
{
#if defined(__WINDOWS__)
	return _stricmp(m_Contents, other.Get());
#elif defined(__APPLE__)
	return strcasecmp(m_Contents, other.Get());
#else
	#error Unknown platform
#endif
}

template<typename CharType, typename Allocator>
template<class>
void String<CharType, Allocator>::Format(const CHAR * fmtString, ...)
{
	va_list args;
	va_start(args, fmtString);
	VFormat(fmtString, args);
	va_end(args);
}

template<typename CharType, typename Allocator>
template<class>
void String<CharType, Allocator>::VFormat(const CHAR * fmtString, va_list arg)
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
	Assign(buffer, buffer + len);

	// Free memory if we allocated any
	if (buffer != stackBuffer)
	{
		FREE(buffer);
	}
}

//-----------------------------------------------------------------------------
