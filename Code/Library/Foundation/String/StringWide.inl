// StringWide.inl
//-----------------------------------------------------------------------------


// String specialization
//-----------------------------------------------------------------------------
template<typename CharType, typename Allocator>
template<typename OtherAllocator, class>
int32 String<CharType, Allocator>::CompareI(const String<WCHAR, OtherAllocator> & other) const
{
	return 0;
}

template<typename CharType, typename Allocator>
template<class>
void String<CharType, Allocator>::Format(const WCHAR * fmtString, ...)
{
}

template<typename CharType, typename Allocator>
template<class>
void String<CharType, Allocator>::VFormat(const WCHAR * fmtString, va_list arg)
{
}

//-----------------------------------------------------------------------------
