#include "StringPlus.h"
#include <Windows.h>
using namespace std;

//从文件读入到string里
std::string ReadFileIntoString(const char *filename)
{
	ifstream ifs(filename);

	//将文件读入到ostringstream对象中
	ostringstream oss;
	char ch;
	while (oss && ifs.get(ch))
		oss.put(ch);

	return oss.str();
}

//在字符串srcStr中找到以startStr开头，以endStr结尾的子字符串，返回该子字符串的下一个字符位置
int FindStr2Str(const std::string &srcStr, size_t pos, const string &startStr, const string &endStr, string &outStr)
{
	size_t s = srcStr.find(startStr, pos);
	if (s == string::npos)
		return -1;
	size_t t = srcStr.find(endStr, s + startStr.length());
	if (t == string::npos)
		return -1;
	outStr = srcStr.substr(s, t - s + endStr.length());
	return t + endStr.length();
}

//在字符串srcStr中找到在startStr与endStr之间的子字符串，返回endStr的下一个字符位置
int FindStr2StrEx(const string &srcStr, size_t pos, const string &startStr, const string &endStr, string &outStr)
{
	size_t s = srcStr.find(startStr, pos);
	if (s == string::npos)
		return -1;
	size_t t = srcStr.find(endStr, s + startStr.length());
	if (t == string::npos)
		return -1;
	outStr = srcStr.substr(s + startStr.length(), t - s - startStr.length());
	return t + endStr.length();
}

//去除字符串中所有的whitespace、&nbsp及</br>;
void DeleteSpaces(string &str)
{
	string newStr;
	for (size_t i = 0; i < str.length(); i++)
	{
		//删除Whitespace
		if (str[i] == '\r' || str[i] == '\n' || str[i] == '\t')
			continue;
		//删除&nbsp;
		else if (str[i] == '&') {
			if (i + 5 < str.length() && str[i + 1] == 'n' && str[i + 2] == 'b'
				&& str[i + 3] == 's' && str[i + 4] == 'p' && str[i + 5] == ';')
			{
				i += 5;
				continue;
			}
			else
				newStr += str[i];
		}
		else
			newStr += str[i];
	}
	str = newStr;
}

//删除srcStr中所有的subStr子字符串
void DeleteSubStr(string &srcStr, const string &subStr)
{
	size_t pos = srcStr.find(subStr);
	while (pos != string::npos)
	{
		srcStr = srcStr.substr(0, pos) + srcStr.substr(pos + subStr.length());
		pos = srcStr.find(subStr);
	}
}
//删除指定的标签，例如：<td width="58"><div align = "center">场序</div></td>，删除td标签后的内容为<div align = "center">场序</div>
void DeleteTag(string &tagStr, const string &tag, string &dataStr)
{
	dataStr = tagStr;
	string startStr, endStr, closeStr;
	startStr = startStr + "<" + tag;
	endStr = ">";
	if (tag == "th")
		//closeStr = "</td>";
		closeStr = "</th>";
	else
		closeStr = closeStr + "</" + tag + ">";
	string subStr;
	FindStr2Str(tagStr, 0, startStr, endStr, subStr);
	DeleteSubStr(dataStr, subStr);
	if (tag != "br")
		DeleteSubStr(dataStr, closeStr);
}

//删除字符串前部跟尾部的空格
void DeleteLeadingTrailingSpace(string &str)
{
	//若字符串为空，则直接返回
	if (str.empty())
	{
		return;
	}
	size_t i = 0, j = str.length() - 1;
	//从前往后找到第一个非空格字符
	while (i <= str.length() - 1 && str[i] == ' ')
	{
		i++;
	}
	if (i == str.length())
	{
		str = "";
		return;
	}
	//从后往前找到第一个非空格字符
	while (str[j] == ' ' && j > i)
	{
		j--;
	}

	//提取子字符串
	str = str.substr(i, j - i + 1);
}

//统计子字符串在源字符串中出现的次数
int SubStrCount(const string &src, const string &target)
{
	int count = 0;
	//char *s = src.c_str(), *t = target.c_str();
	size_t pos = src.find(target, 0);
	while (pos != string::npos)
	{
		count++;
		pos = src.find(target, pos + target.length());
	}
	return count;
}

wstring UTF8ToUnicode(const string& str)
{
	int len = str.length();
	int unicodeLen = ::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

	wchar_t * pUnicode;
	pUnicode = new wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1)*sizeof(wchar_t));

	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, (LPWSTR)pUnicode, unicodeLen);


	wstring rt = (wchar_t*)pUnicode;
	delete pUnicode;

	return rt;
}

string UnicodeToANSI(const wstring& str)
{
	char* pElementText;
	int iTextLen;

	// wide char to multi char
	iTextLen = WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, NULL, 0, NULL, NULL);

	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, sizeof(char)* (iTextLen + 1));

	WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, pElementText, iTextLen, NULL, NULL);

	string strText = pElementText;
	delete[] pElementText;

	return strText;
}

string UTF8ToANSI(const string& str)
{
	wstring wstr = UTF8ToUnicode(str);
	string result = UnicodeToANSI(wstr);
	return result;
}