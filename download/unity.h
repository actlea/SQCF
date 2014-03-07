#pragma once

#include "stdafx.h"
using std::string;

//公共变量
#pragma region 公共的常量
const string file = "D:\\zjm\\nation_fb_info\\";	//存储文件的目录
const string of_country_zh_en_name = file + "of_country_zh_en_name.txt"; //原存储国家的中英文名
const string nf_country_zh_en_name = file + "nf_country_zh_en_name.txt"; //原存储国家的中英文名
const string cLink = file + "cLink.txt";	//从sina网页中爬下来的包含各个国家国家队信息的网址信息
const string sinaPage = file + "sinaPage\\";

const string nationalTeam = file + "nationalTeam.txt";	//国家队信息
//网址
const string wikiURL = "zh.wikipedia.org/zh-cn/%E5%9C%8B%E5%AE%B6%E8%B6%B3%E7%90%83%E9%9A%8A%E5%88%97%E8%A1%A8";
const string nFteamURL = "www.national-football-teams.com";	//提取国家名称，国旗图片和国家队球员

#pragma endregion

extern std::set<string> CTKey;//存放从用户定义的字典中的足球关键字

//返回str中tag的个数
/*
int count(string str, string tag)
{
	int num=0;
	string::size_type  ePos = str.find_last_of(tag);
	string::size_type  bPos = str.find_first_of(tag);
	
	if(bPos==string::npos)
		return 0;
	if(ePos==bPos)
		return 1;
	while(ePos!=bPos)
	{
		ePos = str.find(tag,ePos);
		num++;
	}
	return num;
}*/