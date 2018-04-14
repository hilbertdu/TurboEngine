// StringConvert.inl
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include <codecvt>
#include <string>


namespace StringCvt
{
	AString ConvertToUTF8(const AStringView & str, const char * localeName)
	{
		//构造GBK与wstring间的转码器（wstring_convert在析构时会负责销毁codecvt_byname，所以不用自己delete）
		std::wstring_convert<std::codecvt_byname<wchar_t, char, mbstate_t>> cv1(new std::codecvt_byname<wchar_t, char, mbstate_t>(localeName));
		std::wstring tmpWstr = cv1.from_bytes(str.Get());

		std::wstring_convert<std::codecvt_utf8<wchar_t>> cv2;
		std::string utf8Str = cv2.to_bytes(tmpWstr);

		return AString(utf8Str.c_str());
	}
}

//------------------------------------------------------------------------------