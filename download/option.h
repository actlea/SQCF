/**
*���ļ���Ҫ�洢����һЩ��������
*option.h
*/

#pragma once

#include "stdafx.h"
#include "download.h"
#include <sstream>

//����source��bTag��eTag֮����ַ���������ʵ�ͽ�����ǩ
string GetTag(const string& source, const string& bTag, const string& eTag,int pos=0);
//�ж������Ƿ���������Ҫ������
bool JudgeFootballInfo(const string&); 
//��һ���������ӵ�ê�ı��еĳ�������ȡ����
string SelectURL(const string& AnchorText);

//////////////////////////////////////////////////////////////////////////////////////////
//��ȡwiki�еı������
void DoCountryInfo(); //���������Ϣ
void DeleteStr(string& src, const string& btag, const string& etag);//ɾ��src��btag��etag֮������ݣ�����btag��etag

//int to string function
string itos(int arg);

//��ȡsina��ҳ�й��ڹ��Ҷ����ӵ�id��
void GetCountryTeamId();

//�ж��Ƿ��ǹ�������
bool JudgeCountryName(const string& str);

//��ʼ�������������Ƶ��ĵ�
bool InitCnameFile(const string& filename);

