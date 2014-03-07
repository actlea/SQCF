#include "Table.h"
#include "StringPlus.h"
#include <Urlmon.h>
#include <fstream>
#include <regex>
#pragma comment(lib, "Urlmon.lib")
using namespace std;

std::vector<std::string> dict;

//��ȡ���������е�Ԫ��Ĳ�������
int ExtractTableDataEx(const char *inPath, const char *outPath)
{
	//�������Table���ļ���תΪstring�Է��㴦��
	string table = ReadFileIntoString(inPath);

	//ȥ������еĿո�
	DeleteSpaces(table);

	//ͳ�Ʊ������
	int rowNum = SubStrCount(table, "<tr");


	//�Ա��ǰ���е����������Ϊ��������
	int colNum = 0;
	size_t rpos = 0;
	string rowStr;
	for (int i = 0; i < rowNum; i++)
	{
		rpos = FindStr2Str(table, rpos, "<tr", "</tr>", rowStr);
		int n = SubStrCount(rowStr, "<td");
		if (n > colNum)
			colNum = n;
	}

	
	//��ʼ���������б�����ݵĶ�άvector
	vector< vector<string> > tabVec(rowNum);
	vector< vector<string> >::iterator rIter = tabVec.begin();
	for (; rIter != tabVec.end(); ++rIter)
	{
		(*rIter).resize(colNum);
	}

	//���б������
	rpos = 0;
	for (int i = 0; i < rowNum; i++)
	{
		rpos = FindStr2Str(table, rpos, "<tr", "</tr>", rowStr);
		//������һ�е�����
		int crrntColNum = SubStrCount(rowStr, "<td") + SubStrCount(rowStr, "<th");
		
		//��ÿһ��td���д���
		string tdStr, dataStr, tmpStr;
		int cpos = 0;
		for (int j = 0; j < crrntColNum; j++)
		{
			if (rowStr.find("<th") != string::npos)
				cpos = FindStr2Str(rowStr, cpos, "<th", "</th>", tdStr);
			else
				cpos = FindStr2Str(rowStr, cpos, "<td", "</td>", tdStr);
			
			//ȷ���õ�Ԫ���������
			int spanNum;
			if (tdStr.find("rowspan") != string::npos)
			{
				string spanNumStr;
				FindStr2StrEx(tdStr, 0, "rowspan=\"", "\"", spanNumStr);
				spanNum = atoi(spanNumStr.c_str());
			}
			else
				spanNum = 1;
			//Ϊ�˷�ֹ��ҳ����rowspan������tabVec����Խ�磬��spanNum�޸�Ϊ��ȷֵ
			//�����������http://sports.sina.com.cn/z/wvbach11/schedule/index.shtml ��9������9.23(����)����ֶ�����
			if (i + spanNum - 1 > rowNum - 1)
				spanNum = rowNum - i;
			//��ȡ��Ԫ�������
			ExtractDataFromTd(tdStr, dataStr);

			
			//�ڵ�i�е�vector���ҵ���һ��Ϊ�յ�λ��
			int emptyPos = 0;
			while (tabVec[i][emptyPos] != "")
				emptyPos++;
			//��spanNum��д�뵥Ԫ������dataStr
			for (int k = i; k - i < spanNum; k++)
			{
				
				tabVec[k][emptyPos] = dataStr;
			}
		}
	}

	//����άvectorд���ļ�
	ofstream ofs(outPath, ofstream::out | ofstream::app);
	
	rIter = tabVec.begin();
	for (; rIter != tabVec.end(); ++rIter)
	{
		vector<string>::iterator cIter = (*rIter).begin();
		for (; cIter <= (*rIter).end() - 1; ++cIter)
		{
			//�������һ�����һ�Ϊ��
			if (cIter != (*rIter).end() - 1 && *cIter != "" && *(cIter+1) != "")
				ofs << (*cIter) << ',';
			else
			{
				ofs << (*cIter);
				break;
			}
		}
		ofs << '\n';
	}
	ofs.close();

	return 0;
}


int FindTableInHtmlString(string &htmlStr, string &tabStr)
{
	/*//����ҳ�������з���Ҫ��ı����ַ���
	string allTables;*/

	int nestLevel = 0; //table��ǩ��Ƕ�׼���

	//�ҵ���һ��<table>��ǩ��λ��s
	size_t s = htmlStr.find("<table");
	if (s == string::npos)
	{
		//cerr << "�����޷��ҵ�table��ǩ" << endl;
		return -1;
	}

	//�ҵ���һ��</table>��ǩ��λ��t
	size_t t = htmlStr.find("</table>");
	if (t == string::npos)
	{
		//cerr << "���󣺱���ǩ������" << endl;
		return -1;
	}

	//��ȡ��һ��<table>�͵�һ��</table>֮��Ĳ��֣�������ǩ
	string table = htmlStr.substr(s, t - s + strlen("</table>"));

	//�жϱ��Ƕ�����
	while (true)
	{
		size_t u = table.find("<table", strlen("<table"));
		//û��Ƕ�����˳�ѭ��
		if (u == string::npos)
			break;
		//����Ƕ�ף����³�ȡ���
		table = table.substr(u, string::npos);
		//Ƕ�׼�����1
		nestLevel++;
	}

	int tableInfoType = TableInfoType(table);
	//��������������Ϣ���򷵻�
	if (tableInfoType == 0/* || tableInfoType == 1*/)
	{
		//���������
		tabStr = table;
		//��htmlStr��Ϊʣ�ಿ��
		for (int i = 0; i < nestLevel + 1; i++) //nestLevel+1����Ϊ����Ƕ�ף�����Ҫȥ��һ��</table>
		{
			s = htmlStr.find("</table>", t);
			htmlStr = htmlStr.substr(s + strlen("</table>"));
			//t = s + strlen("</table>");
			t = 0;
		}
		return 0;
	}
	else
	{
		//���ѵ����ַ���ĩ�ˣ�˵����ҳ�ַ���������������Ϣ
		if (nestLevel == 0 && (t + strlen("</table>")) == htmlStr.length())
		{
			//cerr << "����ҳ�治����������Ϣ" << endl;
			return -1;
		}
		//�������Ƕ�׼�����ȡԭʼ��ҳ�ַ��������ಿ��
		else
		{
			for (int i = 0; i < nestLevel + 1; i++) //nestLevel+1����Ϊ����Ƕ�ף�����Ҫȥ��һ��</table>
			{
				s = htmlStr.find("</table>", t);
				htmlStr = htmlStr.substr(s + strlen("</table>"), string::npos);
				//t = s + strlen("</table>");
				t = 0;
			}
		}
		//�ݹ����FindTableInHtmlString
		return FindTableInHtmlString(htmlStr, tabStr);
	}
}


int TableInfoType(const string &table) //-1��ʾû����Ϣ��0��ʾ������Ϣ��1��ʾ��ϸ��Ϣ
{
	//�ҵ���ͷ
	size_t s = table.find("<tr"), t = table.find("</tr>");
	if (s == string::npos || t == string::npos)
		return -1;
	string heading = table.substr(s, t - s + strlen("</tr>"));

	//ͳ�Ʊ�ͷ������
	int colNum = SubStrCount(heading, "<td") + SubStrCount(heading, "<th");
	//���ñ�ͷ���������ڵ���1��������һ����Ϊ��ͷ
	if (colNum <= 1)
	{
		s = table.find("<tr", s + strlen("<tr")), t = table.find("</tr>", t + strlen("</tr>"));
		if (s == string::npos || t == string::npos)
			return -1;
		heading = table.substr(s, t - s + strlen("</tr>")); //�ҵ���ͷ
		colNum = SubStrCount(heading, "<td");
	}

	int match = 0;
	for (vector<string>::iterator it = dict.begin(); it != dict.end(); it++)
	{
		int cnt = SubStrCount(heading, *it);
		match += cnt;
	}

	//����ƥ��̶��ж��Ƿ���������Ϣ
	if (match * 1.0 / colNum >= 0.6 /*&& detailDictMatch == 0*/)
		return 0;
	/*else if (match * 1.0 / colNum >= 0.5 && detailDictMatch > 0)
		return 1;*/
	else
		return -1;
}

int InitDict()
{
	//��ʼ�����´ʵ�
	ifstream ifs;
	char word[20];
	ifs.open(".\\Dict.txt");
	if (!ifs)
	{
		cerr << "�ʵ��ļ���ʧ��" << endl;
		return -1;
	}
	while (ifs.get(word, sizeof(word)))
	{
		dict.push_back(word);
		ifs.get();
	}
	if (!dict.empty())
	{
		cerr << "�ʵ��ʼ���ɹ�" << endl;
	}
	else
	{
		cout << "�ʵ��ʼ��ʧ��" << endl;
		return -1;
	}
	ifs.close();

	return 0;
}

//ȡ���õ�Ԫ������ݣ�ȥ������ı�ǩ
void ExtractDataFromTd(string &tdStr, string &dataStr)
{
	string tmpStr;
	dataStr = tdStr;
	//ɾ��td��ǩ
	while (dataStr.find("<td") != string::npos)
	{
		DeleteTag(dataStr, "td", tmpStr);
		dataStr = tmpStr;
	}
	//ɾ��th��ǩ
	while (dataStr.find("<th") != string::npos)
	{
		DeleteTag(dataStr, "th", tmpStr);
		dataStr = tmpStr;
	}
	//����div��ǩ��ɾ��
	while (dataStr.find("<div") != string::npos)
	{
		DeleteTag(dataStr, "div", tmpStr);
		dataStr = tmpStr;
	}
	//����strong��ǩ��ɾ��
	while (dataStr.find("<strong") != string::npos)
	{
		DeleteTag(dataStr, "strong", tmpStr);
		dataStr = tmpStr;
	}
	//����sup��ǩ��ɾ��
	while (dataStr.find("<sup") != string::npos)
	{
		DeleteTag(dataStr, "sup", tmpStr);
		dataStr = tmpStr;
	}
	//����span��ǩ��ɾ��
	while (dataStr.find("<span") != string::npos)
	{
		DeleteTag(dataStr, "span", tmpStr);
		dataStr = tmpStr;
	}
	//����p��ǩ��ɾ��
	while (dataStr.find("<p") != string::npos)
	{
		DeleteTag(dataStr, "p", tmpStr);
		dataStr = tmpStr;
	}
	//����br��ǩ��ɾ��
	while (dataStr.find("<br") != string::npos)
	{
		DeleteTag(dataStr, "br", tmpStr);
		dataStr = tmpStr;
	}
	//ɾ���ֶ�ǰ��Ŀո�
	DeleteLeadingTrailingSpace(dataStr);
	if (dataStr.find(",")!= string::npos)
		int bp=1;
	//ɾ����β�Ķ��š�,����������������ȡʱ����
	if (dataStr.find(",") != string::npos) //���ж��Ƿ��ж��ţ���ֹ�±�Խ��
	{
		if (dataStr[dataStr.length()-1] == ',')
			dataStr = dataStr.substr(0, dataStr.length()-1);
	}
	//���м�Ķ����滻Ϊ'/'��������ë��ַ��и����ȷ�
	regex e(",");
	dataStr = regex_replace(dataStr, e, string("/"));
		
	//Ϊ�˺����жϵ�Ԫ���Ƿ��������ݣ�������һ���ո���䵥Ԫ������
	if (dataStr == "")
		dataStr = " ";
}

void DownloadHtmlFile(const string &url, const string &filePath)
{
	::CoInitialize(NULL);

	//����ҳ��
	cout << "����������ҳ��" << url << endl;
	if (S_OK == URLDownloadToFile(NULL, url.c_str(), filePath.c_str(), 0, NULL))
		std::cout << "ҳ�����سɹ�" << std::endl;
	else
		std::cout << "ҳ������ʧ��" << std::endl;
}

//�Ӹ�����Url����ȡ����Ҫ��ı������
void ExtractTableDataFromUrl(const string &url, const string &tdPath)
{
	//TDOO:�޸���������ҳ�ĳ���
	
	//::CoInitialize(NULL);

	//const char *originalPageFile = ".\\OriginalPage.txt";
	const char *tableFile = ".\\Table.txt";

	//����ҳ��
	//cout << "����������ҳ" << url << endl;
	//if (S_OK == URLDownloadToFile(NULL, url.c_str(), originalPageFile, 0, NULL))
	//	std::cout << "ҳ�����سɹ�" << std::endl;
	//else
	//	std::cout << "ҳ������ʧ��" << std::endl;

	//����ԭʼҳ���ļ���תΪstring�Է��㴦��
	//string originalPageString = ReadFileIntoString(originalPageFile);
	string originalPageString = DownLoadPage(url);
	if(originalPageString.compare("error")==0)
	{
		cout << url << "...��ҳ����ʧ��..." << endl;
		return;
	}
	//��ΪUTF-8���룬��תΪANSI
	if (originalPageString.find("charset=utf-8") != string::npos || 
		originalPageString.find("charset=UTF-8")!= string::npos ||
		originalPageString.find("charset=\"UTF-8\"")!= string::npos ||
		originalPageString.find("charset=\"utf-8\"")!= string::npos)
	{
		cout << "���ڽ�UTF-8����תΪANSI..." << endl;
		originalPageString = UTF8ToANSI(originalPageString);
		cout << "����ת�����" << endl;
	}

	cout << "������ȡ�������..." << endl;
	//��ȡҳ���е���������Ҫ��ı��
	string tabStr;
	//��յ�ǰTableData.txt�е�����
	ofstream tdfs(tdPath, ofstream::trunc | ofstream::out);
	tdfs.close();
	bool found = false;
	while (FindTableInHtmlString(originalPageString, tabStr) == 0)
	{
		//cout << tabStr << endl;
		//�򿪴洢��ǰ�����ļ�
		ofstream tfs(tableFile);
		if (!tfs)
			cerr << "�޷����ļ���" << tableFile << endl;
		tfs << tabStr;
		tfs.close();
		//�ӱ������ȡ����
		try{
		ExtractTableDataEx(tableFile, tdPath.c_str());
		}catch(exception e)
		{
			
		}
		ofstream tdfs(tdPath, ofstream::app | ofstream::out);
		tdfs << endl;
		tdfs.close();
		found = true;
	}
	if (!found)
	{
		cerr << "����ҳ�治����ָ���ı��" << endl;
		return;
	}
	cout << "���������ȡ�ɹ�" << endl;
	return;
}



//�Ӹ�����html�ļ�����ȡ����Ҫ��ı������
void ExtractTableDataFromFile(const string &htmlPath, const string &tdPath)
{
	::CoInitialize(NULL);

	const char *originalPageFile = ".\\OriginalPage.txt";
	const char *tableFile = ".\\Table.txt";

	//����ԭʼҳ���ļ���תΪstring�Է��㴦��
	string originalPageString = ReadFileIntoString(originalPageFile);
	//��ΪUTF-8���룬��תΪANSI
	if (originalPageString.find("charset=utf-8") != string::npos)
	{
		cout << "���ڽ�UTF-8����תΪANSI..." << endl;
		originalPageString = UTF8ToANSI(originalPageString);
		cout << "����ת�����" << endl;
	}

	cout << "������ȡ�������..." << endl;
	//��ȡҳ���е���������Ҫ��ı��
	string tabStr;
	//��յ�ǰTableData.txt�е�����
	ofstream tdfs(tdPath, ofstream::trunc | ofstream::out);
	tdfs.close();
	bool found = false;
	while (FindTableInHtmlString(originalPageString, tabStr) == 0)
	{
		//cout << tabStr << endl;
		//�򿪴洢��ǰ�����ļ�
		ofstream tfs(tableFile);
		if (!tfs)
			cerr << "�޷����ļ���" << tableFile << endl;
		tfs << tabStr;
		tfs.close();
		//�ӱ������ȡ����
		ExtractTableDataEx(tableFile, tdPath.c_str());
		ofstream tdfs(tdPath, ofstream::app | ofstream::out);
		tdfs << endl;
		tdfs.close();
		found = true;
	}
	if (!found)
	{
		cerr << "����ҳ�治����ָ���ı��" << endl;
		return;
	}
	cout << "���������ȡ�ɹ�" << endl;
	return;
}
/*
//�Ӱ���<tr>Ԫ�ص��ַ����г�ȡ�����ֶΣ��ֶ�֮���ö���","�ָ�
int ExtractTableRow(const string &rowStr, string &csStr)
{
//����ַ���csStr�ÿ�
csStr = "";
string rowStrMod = rowStr;
DeleteSpaces(rowStrMod);

//������һ�е�����
int colCount = SubStrCount(rowStrMod, "<td");

size_t pos = 0;
string tdStr, dataStr, tmpStr;

//��ÿһ�н��д���
for (int i = 0; i < colCount; i++)
{
//�ҵ���һ��tdԪ��
pos = FindStr2Str(rowStrMod, pos, "<td", "</td>", tdStr);
if (pos == -1)
return -1;
//size_t tdPos = 0;
//while (1)
//{
//	tdPos = FindStr2StrEx(tdStr, tdPos, ">", "<", dataStr);
//	if (tdPos == -1)
//		return -1;
//	if (dataStr != "")
//	{
//		if (dataStr == "&nbsp;")
//			dataStr = "";
//		csStr += dataStr;
//		break;
//	}
//}
//ɾ��td��ǩ
DeleteTag(tdStr, "td", dataStr);
//����div��ǩ��ɾ��
if (dataStr.find("<div") != string::npos)
{
DeleteTag(dataStr, "div", tmpStr);
dataStr = tmpStr;
}
//����strong��ǩ��ɾ��
if (dataStr.find("<strong") != string::npos)
{
DeleteTag(dataStr, "strong", tmpStr);
dataStr = tmpStr;
}
//����span��ǩ��ɾ��
if (dataStr.find("<span") != string::npos)
{
DeleteTag(dataStr, "span", tmpStr);
dataStr = tmpStr;
}
//����br��ǩ��ɾ��
if (dataStr.find("<br") != string::npos)
{
DeleteTag(dataStr, "br", tmpStr);
dataStr = tmpStr;
}

//ɾ���ֶ�ǰ��Ŀո�
DeleteLeadingTrailingSpace(dataStr);

csStr += dataStr;

if (i != colCount - 1)
csStr += ",";
else
csStr += "\n";
}

return 0;
}
*/

/*
int ExtractTableFromHtml(const char *inPath, const char *outPath)
{
//����ԭʼҳ���ļ���תΪstring�Է��㴦��
string originalPageString = ReadFileIntoString(inPath);
//��ΪUTF-8���룬��תΪANSI
if (originalPageString.find("charset=utf-8") != string::npos)
{
cout << "���ڽ�UTF-8����תΪANSI..." << endl;
originalPageString = UTF8ToANSI(originalPageString);
cout << "����ת�����" << endl;
}

//��ȡ������������Ϣ�ı��
string table = FindTableInHtmlString(originalPageString);

//���ԣ���table�ַ���д���ļ����鿴table��������
ofstream ofs(outPath);
ofs << table;
ofs.close();
//����

if (table == "")
return -1;
else
return 0;
}
*/

//�����ĺ���
//string FindTableInHtmlString(string &htmlStr)
//{
//	/*//����ҳ�������з���Ҫ��ı����ַ���
//	string allTables;*/
//
//	int nestLevel = 0; //table��ǩ��Ƕ�׼���
//
//	//�ҵ���һ��<table>��ǩ��λ��s
//	size_t s = htmlStr.find("<table");
//	if (s == string::npos)
//	{
//		cerr << "�����޷��ҵ�table��ǩ" << endl;
//		return "";
//	}
//
//	//�ҵ���һ��</table>��ǩ��λ��t
//	size_t t = htmlStr.find("</table>");
//	if (t == string::npos)
//	{
//		cerr << "���󣺱���ǩ������" << endl;
//		return "";
//	}
//
//	//��ȡ��һ��<table>�͵�һ��</table>֮��Ĳ��֣�������ǩ
//	string table = htmlStr.substr(s, t - s + strlen("</table>"));
//
//	//�жϱ��Ƕ�����
//	while (true)
//	{
//		size_t u = table.find("<table", strlen("<table"));
//		//û��Ƕ�����˳�ѭ��
//		if (u == string::npos)
//			break;
//		//����Ƕ�ף����³�ȡ���
//		table = table.substr(u, string::npos);
//		//Ƕ�׼�����1
//		nestLevel++;
//	}
//
//	int tableInfoType = TableInfoType(table);
//	//��������������Ϣ������table�ַ���ǰ������������Ϣ������
//	if (tableInfoType == 0/* || tableInfoType == 1*/)
//	{
//		//��ʱ�����ñ���������͹���
//		//string typeStr = (tableInfoType == 0) ? "Event\n" : "Detail\n";
//		//table = typeStr + table;
//
//		return table;
//	}
//	else
//	{
//		//���ѵ����ַ���ĩ�ˣ�˵����ҳ�ַ���������������Ϣ
//		if (nestLevel == 0 && (t + strlen("</table>")) == htmlStr.length())
//		{
//			cerr << "����ҳ�治����������Ϣ" << endl;
//			return "";
//		}
//		//�������Ƕ�׼�����ȡԭʼ��ҳ�ַ��������ಿ��
//		else
//		{
//			for (int i = 0; i < nestLevel + 1; i++) //nestLevel+1����Ϊ����Ƕ�ף�����Ҫȥ��һ��</table>
//			{
//				s = htmlStr.find("</table>", t);
//				htmlStr = htmlStr.substr(s + strlen("</table>"), string::npos);
//				t = s + strlen("</table>");
//			}
//		}
//		//�ݹ����FindTableInHtmlString
//		return FindTableInHtmlString(htmlStr);
//	}
//}

/*
int ExtractTableData(const char *inPath, const char *outPath)
{
//�������Table���ļ���תΪstring�Է��㴦��
string table = ReadFileIntoString(inPath);

int rowsRemain = SubStrCount(table, "<tr");

//�ҵ���ͷ
size_t s = table.find("<tr"), t = table.find("</tr>");
if (s == string::npos || t == string::npos)
return -1;
string heading = table.substr(s, t - s + strlen("</tr>"));
rowsRemain--;

//ͳ�Ʊ�ͷ������
int colNum = SubStrCount(heading, "<td");
//���ñ�ͷ���������ڵ���1��������һ����Ϊ��ͷ
if (colNum <= 1)
{
s = table.find("<tr", s + strlen("<tr")), t = table.find("</tr>", t + strlen("</tr>"));
heading = table.substr(s, t - s + strlen("</tr>")); //�ҵ���ͷ
colNum = SubStrCount(heading, "<td");
rowsRemain--;
}

//��ʼ��ȡ�������
string rowStr, csStr, outStr;
ExtractTableRow(heading, csStr);
outStr += csStr;

size_t pos = t + strlen("</tr>");
for (int i = rowsRemain; i > 0; i--)
{
pos = FindStr2Str(table, pos, "<tr", "</tr>", rowStr);
ExtractTableRow(rowStr, csStr);
outStr += csStr;
}

//����ȡ��������д���ļ�
ofstream ofs(outPath);
ofs << outStr;
ofs.close();

return 0;
}
*/