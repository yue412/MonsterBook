#include "common.h"
#include <stdarg.h>
#include <algorithm>
#include <clocale>
#include <vector>
#include <Windows.h>
#include <fstream>
#include <vector>

std::wstring _FormatWstring(const wchar_t * lpcwszFormat, va_list _list)
{
	size_t nLength = _vscwprintf(lpcwszFormat, _list) + 1; //获取格式化字符串长度
	wchar_t* pszStr = new wchar_t[nLength];
	memset(pszStr, L'\0', nLength);
	_vsnwprintf_s(pszStr, nLength, nLength, lpcwszFormat, _list);
	std::wstring strResult(pszStr);
	delete[]pszStr;
	return strResult;
}

std::wstring FormatWstring(const wchar_t * lpcwszFormat, ...)
{
	std::wstring strResult;
	if (NULL != lpcwszFormat)
	{
		va_list marker = NULL;
		va_start(marker, lpcwszFormat); //初始化变量参数
		strResult = _FormatWstring(lpcwszFormat, marker);
		va_end(marker); //重置变量参数
	}
	return strResult;
}

std::wstring FormatWstring(const std::wstring sFormat, ...)
{
	va_list _va_list;
	va_start(_va_list, sFormat);      /* 初始化变长参数列表 */
	std::wstring sResult = _FormatWstring(sFormat.c_str(), _va_list);    /* 传递变长参数 */
	va_end(_va_list);         /* 结束使用变长参数列表 */
	return sResult;
}

std::wstring UpperString(const std::wstring & sStr)
{
	std::wstring sTmp = sStr;
	transform(sTmp.begin(), sTmp.end(), sTmp.begin(), towupper);
	return sTmp;
}

std::wstring LowerString(const std::wstring & sStr)
{
	std::wstring sTmp = sStr;
	transform(sTmp.begin(), sTmp.end(), sTmp.begin(), towlower);
	return sTmp;
}

// 把一个wstring转化为string
std::string toString(const std::wstring & src)
{
	char* loc = std::setlocale(LC_CTYPE, "");

	size_t const mbs_len = wcstombs(NULL, src.c_str(), 0);
	std::vector<char> tmp(mbs_len + 1);
	wcstombs(&tmp[0], src.c_str(), tmp.size());
	std::string dest;
	dest.assign(tmp.begin(), tmp.end() - 1);
	std::setlocale(LC_CTYPE, loc);
	return dest;
}

// 把一个string转化为wstring
std::wstring toWstring(const std::string& src)
{
	//   std::setlocale(LC_CTYPE, "");
	char* loc = std::setlocale(LC_CTYPE, "zh_CN");

	size_t const wcs_len = mbstowcs(NULL, src.c_str(), 0);
	std::vector<wchar_t> tmp(wcs_len + 1);
	mbstowcs(&tmp[0], src.c_str(), src.size());

	std::wstring dest;
	dest.assign(tmp.begin(), tmp.end() - 1);
	std::setlocale(LC_CTYPE, loc);

	return dest;
}


std::wstring getExePath()
{
	char exeFullPath[MAX_PATH]; // MAX_PATH在WINDEF.h中定义了，等于260
	memset(exeFullPath, 0, MAX_PATH);

	GetModuleFileName(NULL, exeFullPath, MAX_PATH);
	char *p = /*wcsrchr*/strrchr(exeFullPath, '\\');
	*p = 0x00;
	return  toWstring(exeFullPath);
}

bool isRelativePath(const std::wstring & sPath)
{
	return sPath.find(L":") == -1;
}

std::wstring getFullPath(const std::wstring & sPath)
{
	if (isRelativePath(sPath))
	{
		return getExePath() + L"\\" + sPath;
	}
	else
	{
		return sPath;
	}
}

wchar_t * getWC(const char *c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize);

	return wc;
}

std::string UnicodeToUtf8(const std::wstring & str)
{
	// unicode to UTF8
	//预转换，得到所需空间的大小，这次用的函数和上面名字相反
	int u8Len = ::WideCharToMultiByte(CP_UTF8, NULL, str.c_str(), wcslen(str.c_str()), NULL, 0, NULL, NULL);
	//同上，分配空间要给'\0'留个空间
	//UTF8虽然是Unicode的压缩形式，但也是多字节字符串，所以可以以char的形式保存
	char* szU8 = new char[u8Len + 1];
	//转换
	//unicode版对应的strlen是wcslen
	::WideCharToMultiByte(CP_UTF8, NULL, str.c_str(), wcslen(str.c_str()), szU8, u8Len, NULL, NULL);
	//最后加上'\0'
	szU8[u8Len] = '\0';
	std::string sResult = szU8;
	delete[] szU8;
	return sResult;
}

std::wstring Utf8ToUnicode(const std::string& str)
{
	//UTF8 to Unicode
	//预转换，得到所需空间的大小
	int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, str.c_str(), strlen(str.c_str()), NULL, 0);
	//分配空间要给'\0'留个空间，MultiByteToWideChar不会给'\0'空间
	wchar_t* wszString = new wchar_t[wcsLen + 1];
	//转换
	::MultiByteToWideChar(CP_UTF8, NULL, str.c_str(), strlen(str.c_str()), wszString, wcsLen);
	//最后加上'\0'
	wszString[wcsLen] = '\0';
	std::wstring sResult = wszString;
	delete[] wszString;
	return sResult;
}

std::fstream & operator<<(std::fstream & out, const std::wstring & s)
{
	out << UnicodeToUtf8(s);
	return out;
}

std::fstream & operator<<(std::fstream & out, const wchar_t * s)
{
	out << UnicodeToUtf8(s);
	return out;
}

bool fileExists(const std::wstring & sFile)
{
	std::wfstream _file;
	_file.open(sFile, std::ios::in);
	bool bResult = !_file.fail();
	_file.close();
	return bResult;
}

void split(const std::wstring& sValue, wchar_t chr, std::vector<std::wstring>& oStringList)
{
	auto sTemp = sValue + chr;
	std::size_t nStart = 0;
	auto nIndex = sTemp.find(chr, nStart);
	while (nIndex != std::wstring::npos)
	{
		oStringList.push_back(sTemp.substr(nStart, nIndex));
		nStart = nIndex + 1;
		nIndex = sTemp.find(chr, nStart);
	}
}


EnElementClass Name2Class(std::wstring& sName)
{
    auto itr = std::find(g_sClassNames, g_sClassNames + EC_ALL + 1, sName);
    int n = itr - g_sClassNames;
    return (EnElementClass)n;
}

EnFeatures Name2Feature(std::wstring & sName)
{
    auto itr = std::find(g_sFeatureNames, g_sFeatureNames + EF_ALL + 1, sName);
    int n = itr - g_sFeatureNames;
    return (EnFeatures)n;
}
