/**
*该文件主要存储的是一些辅助函数
*option.h
*/

#pragma once

#include "stdafx.h"
#include "download.h"
#include <sstream>

//返回source中bTag和eTag之间的字符，包含其实和结束标签
string GetTag(const string& source, const string& bTag, const string& eTag,int pos=0);
//判断链接是否是我们需要的链接
bool JudgeFootballInfo(const string&); 
//把一个带超链接的锚文本中的超链接提取出来
string SelectURL(const string& AnchorText);

//////////////////////////////////////////////////////////////////////////////////////////
//提取wiki中的表格数据
void DoCountryInfo(); //处理国家信息
void DeleteStr(string& src, const string& btag, const string& etag);//删除src中btag和etag之间的内容，包括btag和etag

//int to string function
string itos(int arg);

//获取sina网页中关于国家队链接的id号
void GetCountryTeamId();

//判断是否是国家名称
bool JudgeCountryName(const string& str);

//初始化包含国家名称的文档
bool InitCnameFile(const string& filename);

