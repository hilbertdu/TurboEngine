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
		//����GBK��wstring���ת������wstring_convert������ʱ�Ḻ������codecvt_byname�����Բ����Լ�delete��
		std::wstring_convert<std::codecvt_byname<wchar_t, char, mbstate_t>> cv1(new std::codecvt_byname<wchar_t, char, mbstate_t>(localeName));
		std::wstring tmpWstr = cv1.from_bytes(str.Get());

		std::wstring_convert<std::codecvt_utf8<wchar_t>> cv2;
		std::string utf8Str = cv2.to_bytes(tmpWstr);

		return AString(utf8Str.c_str());
	}
}

//------------------------------------------------------------------------------