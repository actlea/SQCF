#include "StringPlus.h"
#include <Windows.h>
using namespace std;

//���ļ����뵽string��
std::string ReadFileIntoString(const char *filename)
{
	ifstream ifs(filename);

	//���ļ����뵽ostringstream������
	ostringstream oss;
	char ch;
	while (oss && ifs.get(ch))
		oss.put(ch);

	return oss.str();
}

//���ַ���srcStr���ҵ���startStr��ͷ����endStr��β�����ַ��������ظ����ַ�������һ���ַ�λ��
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

//���ַ���srcStr���ҵ���startStr��endStr֮������ַ���������endStr����һ���ַ�λ��
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

//ȥ���ַ��������е�whitespace��&nbsp��</br>;
void DeleteSpaces(string &str)
{
	string newStr;
	for (size_t i = 0; i < str.length(); i++)
	{
		//ɾ��Whitespace
		if (str[i] == '\r' || str[i] == '\n' || str[i] == '\t')
			continue;
		//ɾ��&nbsp;
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

//ɾ��srcStr�����е�subStr���ַ���
void DeleteSubStr(string &srcStr, const string &subStr)
{
	size_t pos = srcStr.find(subStr);
	while (pos != string::npos)
	{
		srcStr = srcStr.substr(0, pos) + srcStr.substr(pos + subStr.length());
		pos = srcStr.find(subStr);
	}
}
//ɾ��ָ���ı�ǩ�����磺<td width="58"><div align = "center">����</div></td>��ɾ��td��ǩ�������Ϊ<div align = "center">����</div>
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

//ɾ���ַ���ǰ����β���Ŀո�
void DeleteLeadingTrailingSpace(string &str)
{
	//���ַ���Ϊ�գ���ֱ�ӷ���
	if (str.empty())
	{
		return;
	}
	size_t i = 0, j = str.length() - 1;
	//��ǰ�����ҵ���һ���ǿո��ַ�
	while (i <= str.length() - 1 && str[i] == ' ')
	{
		i++;
	}
	if (i == str.length())
	{
		str = "";
		return;
	}
	//�Ӻ���ǰ�ҵ���һ���ǿո��ַ�
	while (str[j] == ' ' && j > i)
	{
		j--;
	}

	//��ȡ���ַ���
	str = str.substr(i, j - i + 1);
}

//ͳ�����ַ�����Դ�ַ����г��ֵĴ���
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