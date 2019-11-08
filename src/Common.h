#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <fstream>
#include <vector>
#include "MonsterTypes.h"

std::wstring FormatWstring(const wchar_t *lpcwszFormat, ...);
std::wstring FormatWstring(const std::wstring sFormat, ...);
std::wstring UpperString(const std::wstring& sStr);
std::wstring LowerString(const std::wstring& sStr);
std::string toString(const std::wstring & src);
std::wstring toWstring(const std::string& src);
std::wstring getExePath();
bool isRelativePath(const std::wstring& sPath);
std::wstring getFullPath(const std::wstring& sPath);
wchar_t * getWC(const char *c);
std::string UnicodeToUtf8(const std::wstring& str);
std::wstring Utf8ToUnicode(const std::string& str);
std::fstream& operator<<(std::fstream& out, const std::wstring& s);
std::fstream& operator<<(std::fstream& out, const wchar_t* s);
bool fileExists(const std::wstring& sFile);
void split(const std::wstring& sValue, wchar_t chr, std::vector<std::wstring>& oStringList);

EnElementClass Name2Class(std::wstring& sName);
EnFeatures Name2Feature(std::wstring& sName);

#endif // !COMMON_H
