#pragma once

#include "stdafx.h"
using std::string;

//��������
#pragma region �����ĳ���
const string file = "D:\\zjm\\nation_fb_info\\";	//�洢�ļ���Ŀ¼
const string of_country_zh_en_name = file + "of_country_zh_en_name.txt"; //ԭ�洢���ҵ���Ӣ����
const string nf_country_zh_en_name = file + "nf_country_zh_en_name.txt"; //ԭ�洢���ҵ���Ӣ����
const string cLink = file + "cLink.txt";	//��sina��ҳ���������İ����������ҹ��Ҷ���Ϣ����ַ��Ϣ
const string sinaPage = file + "sinaPage\\";

const string nationalTeam = file + "nationalTeam.txt";	//���Ҷ���Ϣ
//��ַ
const string wikiURL = "zh.wikipedia.org/zh-cn/%E5%9C%8B%E5%AE%B6%E8%B6%B3%E7%90%83%E9%9A%8A%E5%88%97%E8%A1%A8";
const string nFteamURL = "www.national-football-teams.com";	//��ȡ�������ƣ�����ͼƬ�͹��Ҷ���Ա

#pragma endregion

extern std::set<string> CTKey;//��Ŵ��û�������ֵ��е�����ؼ���

//����str��tag�ĸ���
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