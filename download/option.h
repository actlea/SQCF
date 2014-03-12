/**
*该文件主要存储的是一些辅助函数
*option.h
*/

#pragma once

#include "stdafx.h"
#include "download.h"
#include <map>
#include <sstream>
#include <algorithm>

//返回source中bTag和eTag之间的字符，包含其实和结束标签
string GetTag(const string& source, const string& bTag, const string& eTag,int flag=0);
//判断链接是否是我们需要的链接
bool JudgeFootballInfo(const string&); 
//把一个带超链接的锚文本中的超链接提取出来
string SelectURL(const string& AnchorText);

//////////////////////////////////////////////////////////////////////////////////////////
//提取wiki中的表格数据
void DoCountryInfo(); //处理国家信息
void DeleteStr(string& src, const string& btag, const string& etag);//删除src中btag和etag之间的内容，包括btag和etag

/////////////////////////////////////////////////////////////////////////////////////////
//int to string function
string itos(int arg);

//获取sina网页中关于国家队链接的id号
void GetCountryTeamId();

//判断是否是国家名称
bool JudgeCountryName(const string& str);

//初始化包含国家名称的文档
bool InitCnameFile();

//提取国家队教练
void ExtractCountryCoach();

/**********************************************************************
	*if there exist a string @source after position @pos like this
	*<a...>ABC</a> or <a...><...>ABC</...></a>
	*then when finish this function value = "ABC", and return position
	* point just after </a> ,verse value is assign to empty and return 
	* return position point right behind </a>
	*source为原字符串	pos:the string in source after pos
	*element_name:标签的名称
***********************************************************************/		
string::size_type NextValue(const  string& source,string::size_type pos,const string& element_name,string& value,bool non_end_mark=false);


/*******************************************************************************
	Function:       NextElement
	Description:    Get fisrt element named @element_name from source after pos
	Calls:          // 被本函数调用的函数清单
	Table Accessed: // 被访问的表（此项仅对于牵扯到数据库操作的程序）
	Table Updated: // 被修改的表（此项仅对于牵扯到数据库操作的程序）
	Input:          1non_end_mark表示要寻找的元素是否没有结束标记，比如<img.....>,如果bool non_end_mark=true
					那么就是类似<a..>...</a>这样的标签
	Output:         // 对输出参数的说明。
	Return:         //如果没有找到元素，则返回source.size()
					//找到则返回这一元素的下一位置
					//因此如果返回值为source.size(),则并不能确定元素是否存在，可以
					//检查element是否为空来判断元素是否存在
	Other:
	*********************************************************************************/
string::size_type NextElement(const  string&source,string::size_type pos,const string&element_name,string&element,bool non_end_mark=false);

//提取标签元素中某个属性的值，仍然分两种类型:<a..></a>或者是<img.../>的类型
string::size_type NextAttribute(const string&source,const string& attribute_name,string &value,string::size_type pos,bool non_end_mark=false);

bool nocase_compare(char c1,char c2);

//判断网址所代表的网页内型
URL UrlFlag(const string&url);

/***********************************************************
 目的:提取网页中的链接
 输入：source为网页源文件
		e 表示网址类型
 返回:map的第一个参数为url,第二个参数为URL类型
 其他：如果html文件中同一个url出现多次，则也只在map中插入一项
 */
std::map<string,URL> GetHyperLinks(const string& source);

/*************************************************************
*Function:get info of national country team
*input:source is page string
		s_nteam is a struct to store the useful info
		if the function succeffully finish,flag is assign to true
*
*/
void CountryPage(const string& source, _sNTeam& s_nteam);

//提取球员信息
void PlayerPage(const string& source,_sPlayer& s_player);

//获取date
void GetDate(_sDate& Date);

//按行读取字符串,返回行结束符的位置
// prama:tableStr为原始的数据文件
//		lineStr为按行从tableStr中读取的字符串
//other：如果按行读取没有换行符了，就返回tableStr.size();
string::size_type ReadLine(const string& tableStr,string& lineStr,string::size_type pos=0);

string::size_type locate(string &line,const string& tag,int index);//定位到第index个tag处,当前行保存在line中。找到则返回true
string::size_type locate(string &line,const string&tag);

string extract(string&line);//提取line中的数据

void eraseTag(string &source,char tag);//删除source中的所有tag标签

//提取表格数据
//从下载好的网页中提取表格存入string中
std::string ExtractDataToStr(string& str);
std::string ExtractTableDataEx2(string& str);

//从本地文件中读取数据存入pageSrc，如果成功返回true
bool ReadFromFile(string filePath,string& pageSrc);
bool WriteToFile(string filePath,string& pageSrc);