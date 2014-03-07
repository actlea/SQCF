#include "Table.h"
#include "StringPlus.h"
#include <Urlmon.h>
#include <fstream>
#include <regex>
#pragma comment(lib, "Urlmon.lib")
using namespace std;

std::vector<std::string> dict;

//提取包含横跨多行单元格的不规则表格
int ExtractTableDataEx(const char *inPath, const char *outPath)
{
	//读入包含Table的文件，转为string以方便处理
	string table = ReadFileIntoString(inPath);

	//去除表格中的空格
	DeleteSpaces(table);

	//统计表格行数
	int rowNum = SubStrCount(table, "<tr");


	//以表格前三行的最大列数作为表格的列数
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

	
	//初始化包含所有表格数据的二维vector
	vector< vector<string> > tabVec(rowNum);
	vector< vector<string> >::iterator rIter = tabVec.begin();
	for (; rIter != tabVec.end(); ++rIter)
	{
		(*rIter).resize(colNum);
	}

	//按行遍历表格
	rpos = 0;
	for (int i = 0; i < rowNum; i++)
	{
		rpos = FindStr2Str(table, rpos, "<tr", "</tr>", rowStr);
		//计算这一行的列数
		int crrntColNum = SubStrCount(rowStr, "<td") + SubStrCount(rowStr, "<th");
		
		//对每一个td进行处理
		string tdStr, dataStr, tmpStr;
		int cpos = 0;
		for (int j = 0; j < crrntColNum; j++)
		{
			if (rowStr.find("<th") != string::npos)
				cpos = FindStr2Str(rowStr, cpos, "<th", "</th>", tdStr);
			else
				cpos = FindStr2Str(rowStr, cpos, "<td", "</td>", tdStr);
			
			//确定该单元格横跨的行数
			int spanNum;
			if (tdStr.find("rowspan") != string::npos)
			{
				string spanNumStr;
				FindStr2StrEx(tdStr, 0, "rowspan=\"", "\"", spanNumStr);
				spanNum = atoi(spanNumStr.c_str());
			}
			else
				spanNum = 1;
			//为了防止网页本身rowspan错误导致tabVec数组越界，将spanNum修改为正确值
			//该情况出现在http://sports.sina.com.cn/z/wvbach11/schedule/index.shtml 第9比赛日9.23(周五)这个字段上面
			if (i + spanNum - 1 > rowNum - 1)
				spanNum = rowNum - i;
			//提取单元格的数据
			ExtractDataFromTd(tdStr, dataStr);

			
			//在第i行的vector中找到第一个为空的位置
			int emptyPos = 0;
			while (tabVec[i][emptyPos] != "")
				emptyPos++;
			//对spanNum行写入单元格数据dataStr
			for (int k = i; k - i < spanNum; k++)
			{
				
				tabVec[k][emptyPos] = dataStr;
			}
		}
	}

	//将二维vector写入文件
	ofstream ofs(outPath, ofstream::out | ofstream::app);
	
	rIter = tabVec.begin();
	for (; rIter != tabVec.end(); ++rIter)
	{
		vector<string>::iterator cIter = (*rIter).begin();
		for (; cIter <= (*rIter).end() - 1; ++cIter)
		{
			//不是最后一项，且下一项不为空
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
	/*//包含页面中所有符合要求的表格的字符串
	string allTables;*/

	int nestLevel = 0; //table标签的嵌套级数

	//找到第一个<table>标签的位置s
	size_t s = htmlStr.find("<table");
	if (s == string::npos)
	{
		//cerr << "错误：无法找到table标签" << endl;
		return -1;
	}

	//找到第一个</table>标签的位置t
	size_t t = htmlStr.find("</table>");
	if (t == string::npos)
	{
		//cerr << "错误：表格标签不完整" << endl;
		return -1;
	}

	//抽取第一个<table>和第一个</table>之间的部分，包含标签
	string table = htmlStr.substr(s, t - s + strlen("</table>"));

	//判断表格嵌套情况
	while (true)
	{
		size_t u = table.find("<table", strlen("<table"));
		//没有嵌套则退出循环
		if (u == string::npos)
			break;
		//若有嵌套，重新抽取表格
		table = table.substr(u, string::npos);
		//嵌套级数加1
		nestLevel++;
	}

	int tableInfoType = TableInfoType(table);
	//若表格包含赛程信息，则返回
	if (tableInfoType == 0/* || tableInfoType == 1*/)
	{
		//将结果返回
		tabStr = table;
		//将htmlStr置为剩余部分
		for (int i = 0; i < nestLevel + 1; i++) //nestLevel+1是因为若无嵌套，则还是要去掉一个</table>
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
		//如已到达字符串末端，说明网页字符串不包含赛程信息
		if (nestLevel == 0 && (t + strlen("</table>")) == htmlStr.length())
		{
			//cerr << "错误：页面不包含赛事信息" << endl;
			return -1;
		}
		//否则根据嵌套级数抽取原始网页字符串的其余部分
		else
		{
			for (int i = 0; i < nestLevel + 1; i++) //nestLevel+1是因为若无嵌套，则还是要去掉一个</table>
			{
				s = htmlStr.find("</table>", t);
				htmlStr = htmlStr.substr(s + strlen("</table>"), string::npos);
				//t = s + strlen("</table>");
				t = 0;
			}
		}
		//递归调用FindTableInHtmlString
		return FindTableInHtmlString(htmlStr, tabStr);
	}
}


int TableInfoType(const string &table) //-1表示没有信息，0表示赛事信息，1表示详细信息
{
	//找到表头
	size_t s = table.find("<tr"), t = table.find("</tr>");
	if (s == string::npos || t == string::npos)
		return -1;
	string heading = table.substr(s, t - s + strlen("</tr>"));

	//统计表头的列数
	int colNum = SubStrCount(heading, "<td") + SubStrCount(heading, "<th");
	//若该表头的列数少于等于1，则用下一行作为表头
	if (colNum <= 1)
	{
		s = table.find("<tr", s + strlen("<tr")), t = table.find("</tr>", t + strlen("</tr>"));
		if (s == string::npos || t == string::npos)
			return -1;
		heading = table.substr(s, t - s + strlen("</tr>")); //找到表头
		colNum = SubStrCount(heading, "<td");
	}

	int match = 0;
	for (vector<string>::iterator it = dict.begin(); it != dict.end(); it++)
	{
		int cnt = SubStrCount(heading, *it);
		match += cnt;
	}

	//根据匹配程度判断是否含有赛事信息
	if (match * 1.0 / colNum >= 0.6 /*&& detailDictMatch == 0*/)
		return 0;
	/*else if (match * 1.0 / colNum >= 0.5 && detailDictMatch > 0)
		return 1;*/
	else
		return -1;
}

int InitDict()
{
	//初始化赛事词典
	ifstream ifs;
	char word[20];
	ifs.open(".\\Dict.txt");
	if (!ifs)
	{
		cerr << "词典文件打开失败" << endl;
		return -1;
	}
	while (ifs.get(word, sizeof(word)))
	{
		dict.push_back(word);
		ifs.get();
	}
	if (!dict.empty())
	{
		cerr << "词典初始化成功" << endl;
	}
	else
	{
		cout << "词典初始化失败" << endl;
		return -1;
	}
	ifs.close();

	return 0;
}

//取出该单元格的数据，去掉多余的标签
void ExtractDataFromTd(string &tdStr, string &dataStr)
{
	string tmpStr;
	dataStr = tdStr;
	//删除td标签
	while (dataStr.find("<td") != string::npos)
	{
		DeleteTag(dataStr, "td", tmpStr);
		dataStr = tmpStr;
	}
	//删除th标签
	while (dataStr.find("<th") != string::npos)
	{
		DeleteTag(dataStr, "th", tmpStr);
		dataStr = tmpStr;
	}
	//如有div标签则删除
	while (dataStr.find("<div") != string::npos)
	{
		DeleteTag(dataStr, "div", tmpStr);
		dataStr = tmpStr;
	}
	//如有strong标签则删除
	while (dataStr.find("<strong") != string::npos)
	{
		DeleteTag(dataStr, "strong", tmpStr);
		dataStr = tmpStr;
	}
	//如有sup标签则删除
	while (dataStr.find("<sup") != string::npos)
	{
		DeleteTag(dataStr, "sup", tmpStr);
		dataStr = tmpStr;
	}
	//如有span标签则删除
	while (dataStr.find("<span") != string::npos)
	{
		DeleteTag(dataStr, "span", tmpStr);
		dataStr = tmpStr;
	}
	//如有p标签则删除
	while (dataStr.find("<p") != string::npos)
	{
		DeleteTag(dataStr, "p", tmpStr);
		dataStr = tmpStr;
	}
	//如有br标签则删除
	while (dataStr.find("<br") != string::npos)
	{
		DeleteTag(dataStr, "br", tmpStr);
		dataStr = tmpStr;
	}
	//删除字段前后的空格
	DeleteLeadingTrailingSpace(dataStr);
	if (dataStr.find(",")!= string::npos)
		int bp=1;
	//删除行尾的逗号‘,’，以免在赛程提取时出错
	if (dataStr.find(",") != string::npos) //先判断是否含有逗号，防止下标越界
	{
		if (dataStr[dataStr.length()-1] == ',')
			dataStr = dataStr.substr(0, dataStr.length()-1);
	}
	//将中间的逗号替换为'/'，如在羽毛球局分中各场比分
	regex e(",");
	dataStr = regex_replace(dataStr, e, string("/"));
		
	//为了后面判断单元格是否已有数据，这里用一个空格填充单元格数据
	if (dataStr == "")
		dataStr = " ";
}

void DownloadHtmlFile(const string &url, const string &filePath)
{
	::CoInitialize(NULL);

	//下载页面
	cout << "正在下载网页：" << url << endl;
	if (S_OK == URLDownloadToFile(NULL, url.c_str(), filePath.c_str(), 0, NULL))
		std::cout << "页面下载成功" << std::endl;
	else
		std::cout << "页面下载失败" << std::endl;
}

//从给定的Url中提取满足要求的表格数据
void ExtractTableDataFromUrl(const string &url, const string &tdPath)
{
	//TDOO:修改了下载网页的程序
	
	//::CoInitialize(NULL);

	//const char *originalPageFile = ".\\OriginalPage.txt";
	const char *tableFile = ".\\Table.txt";

	//下载页面
	//cout << "正在下载网页" << url << endl;
	//if (S_OK == URLDownloadToFile(NULL, url.c_str(), originalPageFile, 0, NULL))
	//	std::cout << "页面下载成功" << std::endl;
	//else
	//	std::cout << "页面下载失败" << std::endl;

	//读入原始页面文件，转为string以方便处理
	//string originalPageString = ReadFileIntoString(originalPageFile);
	string originalPageString = DownLoadPage(url);
	if(originalPageString.compare("error")==0)
	{
		cout << url << "...网页下载失败..." << endl;
		return;
	}
	//若为UTF-8编码，则转为ANSI
	if (originalPageString.find("charset=utf-8") != string::npos || 
		originalPageString.find("charset=UTF-8")!= string::npos ||
		originalPageString.find("charset=\"UTF-8\"")!= string::npos ||
		originalPageString.find("charset=\"utf-8\"")!= string::npos)
	{
		cout << "正在将UTF-8编码转为ANSI..." << endl;
		originalPageString = UTF8ToANSI(originalPageString);
		cout << "编码转换完毕" << endl;
	}

	cout << "正在提取表格数据..." << endl;
	//提取页面中的所有满足要求的表格
	string tabStr;
	//清空当前TableData.txt中的内容
	ofstream tdfs(tdPath, ofstream::trunc | ofstream::out);
	tdfs.close();
	bool found = false;
	while (FindTableInHtmlString(originalPageString, tabStr) == 0)
	{
		//cout << tabStr << endl;
		//打开存储当前表格的文件
		ofstream tfs(tableFile);
		if (!tfs)
			cerr << "无法打开文件：" << tableFile << endl;
		tfs << tabStr;
		tfs.close();
		//从表格中提取数据
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
		cerr << "错误：页面不含有指定的表格" << endl;
		return;
	}
	cout << "表格数据提取成功" << endl;
	return;
}



//从给定的html文件中提取满足要求的表格数据
void ExtractTableDataFromFile(const string &htmlPath, const string &tdPath)
{
	::CoInitialize(NULL);

	const char *originalPageFile = ".\\OriginalPage.txt";
	const char *tableFile = ".\\Table.txt";

	//读入原始页面文件，转为string以方便处理
	string originalPageString = ReadFileIntoString(originalPageFile);
	//若为UTF-8编码，则转为ANSI
	if (originalPageString.find("charset=utf-8") != string::npos)
	{
		cout << "正在将UTF-8编码转为ANSI..." << endl;
		originalPageString = UTF8ToANSI(originalPageString);
		cout << "编码转换完毕" << endl;
	}

	cout << "正在提取表格数据..." << endl;
	//提取页面中的所有满足要求的表格
	string tabStr;
	//清空当前TableData.txt中的内容
	ofstream tdfs(tdPath, ofstream::trunc | ofstream::out);
	tdfs.close();
	bool found = false;
	while (FindTableInHtmlString(originalPageString, tabStr) == 0)
	{
		//cout << tabStr << endl;
		//打开存储当前表格的文件
		ofstream tfs(tableFile);
		if (!tfs)
			cerr << "无法打开文件：" << tableFile << endl;
		tfs << tabStr;
		tfs.close();
		//从表格中提取数据
		ExtractTableDataEx(tableFile, tdPath.c_str());
		ofstream tdfs(tdPath, ofstream::app | ofstream::out);
		tdfs << endl;
		tdfs.close();
		found = true;
	}
	if (!found)
	{
		cerr << "错误：页面不含有指定的表格" << endl;
		return;
	}
	cout << "表格数据提取成功" << endl;
	return;
}
/*
//从包含<tr>元素的字符串中抽取所需字段，字段之间用逗号","分隔
int ExtractTableRow(const string &rowStr, string &csStr)
{
//输出字符串csStr置空
csStr = "";
string rowStrMod = rowStr;
DeleteSpaces(rowStrMod);

//计算这一行的列数
int colCount = SubStrCount(rowStrMod, "<td");

size_t pos = 0;
string tdStr, dataStr, tmpStr;

//对每一列进行处理
for (int i = 0; i < colCount; i++)
{
//找到下一个td元素
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
//删除td标签
DeleteTag(tdStr, "td", dataStr);
//如有div标签则删除
if (dataStr.find("<div") != string::npos)
{
DeleteTag(dataStr, "div", tmpStr);
dataStr = tmpStr;
}
//如有strong标签则删除
if (dataStr.find("<strong") != string::npos)
{
DeleteTag(dataStr, "strong", tmpStr);
dataStr = tmpStr;
}
//如有span标签则删除
if (dataStr.find("<span") != string::npos)
{
DeleteTag(dataStr, "span", tmpStr);
dataStr = tmpStr;
}
//如有br标签则删除
if (dataStr.find("<br") != string::npos)
{
DeleteTag(dataStr, "br", tmpStr);
dataStr = tmpStr;
}

//删除字段前后的空格
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
//读入原始页面文件，转为string以方便处理
string originalPageString = ReadFileIntoString(inPath);
//若为UTF-8编码，则转为ANSI
if (originalPageString.find("charset=utf-8") != string::npos)
{
cout << "正在将UTF-8编码转为ANSI..." << endl;
originalPageString = UTF8ToANSI(originalPageString);
cout << "编码转换完毕" << endl;
}

//提取出包含赛程信息的表格
string table = FindTableInHtmlString(originalPageString);

//测试：将table字符串写入文件，查看table有无问题
ofstream ofs(outPath);
ofs << table;
ofs.close();
//测试

if (table == "")
return -1;
else
return 0;
}
*/

//废弃的函数
//string FindTableInHtmlString(string &htmlStr)
//{
//	/*//包含页面中所有符合要求的表格的字符串
//	string allTables;*/
//
//	int nestLevel = 0; //table标签的嵌套级数
//
//	//找到第一个<table>标签的位置s
//	size_t s = htmlStr.find("<table");
//	if (s == string::npos)
//	{
//		cerr << "错误：无法找到table标签" << endl;
//		return "";
//	}
//
//	//找到第一个</table>标签的位置t
//	size_t t = htmlStr.find("</table>");
//	if (t == string::npos)
//	{
//		cerr << "错误：表格标签不完整" << endl;
//		return "";
//	}
//
//	//抽取第一个<table>和第一个</table>之间的部分，包含标签
//	string table = htmlStr.substr(s, t - s + strlen("</table>"));
//
//	//判断表格嵌套情况
//	while (true)
//	{
//		size_t u = table.find("<table", strlen("<table"));
//		//没有嵌套则退出循环
//		if (u == string::npos)
//			break;
//		//若有嵌套，重新抽取表格
//		table = table.substr(u, string::npos);
//		//嵌套级数加1
//		nestLevel++;
//	}
//
//	int tableInfoType = TableInfoType(table);
//	//若表格包含赛程信息，则在table字符串前面插入表格类别信息并返回
//	if (tableInfoType == 0/* || tableInfoType == 1*/)
//	{
//		//暂时不启用标记赛程类型功能
//		//string typeStr = (tableInfoType == 0) ? "Event\n" : "Detail\n";
//		//table = typeStr + table;
//
//		return table;
//	}
//	else
//	{
//		//如已到达字符串末端，说明网页字符串不包含赛程信息
//		if (nestLevel == 0 && (t + strlen("</table>")) == htmlStr.length())
//		{
//			cerr << "错误：页面不包含赛事信息" << endl;
//			return "";
//		}
//		//否则根据嵌套级数抽取原始网页字符串的其余部分
//		else
//		{
//			for (int i = 0; i < nestLevel + 1; i++) //nestLevel+1是因为若无嵌套，则还是要去掉一个</table>
//			{
//				s = htmlStr.find("</table>", t);
//				htmlStr = htmlStr.substr(s + strlen("</table>"), string::npos);
//				t = s + strlen("</table>");
//			}
//		}
//		//递归调用FindTableInHtmlString
//		return FindTableInHtmlString(htmlStr);
//	}
//}

/*
int ExtractTableData(const char *inPath, const char *outPath)
{
//读入包含Table的文件，转为string以方便处理
string table = ReadFileIntoString(inPath);

int rowsRemain = SubStrCount(table, "<tr");

//找到表头
size_t s = table.find("<tr"), t = table.find("</tr>");
if (s == string::npos || t == string::npos)
return -1;
string heading = table.substr(s, t - s + strlen("</tr>"));
rowsRemain--;

//统计表头的列数
int colNum = SubStrCount(heading, "<td");
//若该表头的列数少于等于1，则用下一行作为表头
if (colNum <= 1)
{
s = table.find("<tr", s + strlen("<tr")), t = table.find("</tr>", t + strlen("</tr>"));
heading = table.substr(s, t - s + strlen("</tr>")); //找到表头
colNum = SubStrCount(heading, "<td");
rowsRemain--;
}

//开始抽取表格数据
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

//将提取出的数据写入文件
ofstream ofs(outPath);
ofs << outStr;
ofs.close();

return 0;
}
*/