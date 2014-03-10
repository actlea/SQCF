/**
*���ļ���Ҫ�洢����һЩ��������
*option.h
*/

#pragma once

#include "stdafx.h"
#include "download.h"
#include <map>
#include <sstream>
#include <algorithm>

//����source��bTag��eTag֮����ַ���������ʵ�ͽ�����ǩ
string GetTag(const string& source, const string& bTag, const string& eTag,int flag=0);
//�ж������Ƿ���������Ҫ������
bool JudgeFootballInfo(const string&); 
//��һ���������ӵ�ê�ı��еĳ�������ȡ����
string SelectURL(const string& AnchorText);

//////////////////////////////////////////////////////////////////////////////////////////
//��ȡwiki�еı������
void DoCountryInfo(); //���������Ϣ
void DeleteStr(string& src, const string& btag, const string& etag);//ɾ��src��btag��etag֮������ݣ�����btag��etag

/////////////////////////////////////////////////////////////////////////////////////////
//int to string function
string itos(int arg);

//��ȡsina��ҳ�й��ڹ��Ҷ����ӵ�id��
void GetCountryTeamId();

//�ж��Ƿ��ǹ�������
bool JudgeCountryName(const string& str);

//��ʼ�������������Ƶ��ĵ�
bool InitCnameFile();

//��ȡ���Ҷӽ���
void ExtractCountryCoach();

/**********************************************************************
	*if there exist a string @source after position @pos like this
	*<a...>ABC</a> or <a...><...>ABC</...></a>
	*then when finish this function value = "ABC", and return position
	* point just after </a> ,verse value is assign to empty and return 
	* return position point right behind </a>
	*sourceΪԭ�ַ���	pos:the string in source after pos
	*element_name:��ǩ������
***********************************************************************/		
string::size_type NextValue(const  string& source,string::size_type pos,const string& element_name,string& value,bool non_end_mark=false);


/*******************************************************************************
	Function:       NextElement
	Description:    Get fisrt element named @element_name from source after pos
	Calls:          // �����������õĺ����嵥
	Table Accessed: // �����ʵı����������ǣ�������ݿ�����ĳ���
	Table Updated: // ���޸ĵı����������ǣ�������ݿ�����ĳ���
	Input:          1non_end_mark��ʾҪѰ�ҵ�Ԫ���Ƿ�û�н�����ǣ�����<img.....>
	Output:         // �����������˵����
	Return:         //���û���ҵ�Ԫ�أ��򷵻�source.size()
					//�ҵ��򷵻���һԪ�ص���һλ��
					//����������ֵΪsource.size(),�򲢲���ȷ��Ԫ���Ƿ���ڣ�����
					//���element�Ƿ�Ϊ�����ж�Ԫ���Ƿ����

	*********************************************************************************/
string::size_type NextElement(const  string&source,string::size_type pos,const string&element_name,string&element,bool non_end_mark=false);

bool nocase_compare(char c1,char c2);

//�ж���ַ���������ҳ����
URL UrlFlag(const string&url);

/***********************************************************
 Ŀ��:��ȡ��ҳ�е�����
 ���룺sourceΪ��ҳԴ�ļ�
		e ��ʾ��ַ����
 ����:map�ĵ�һ������Ϊurl,�ڶ�������ΪURL����
 ���������html�ļ���ͬһ��url���ֶ�Σ���Ҳֻ��map�в���һ��
 */
std::map<string,URL> GetHyperLinks(const string& source);