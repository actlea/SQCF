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

//����source��bTag��eTag֮����ַ�������flag�Ĳ�ͬ�������Ƿ������ʼ�ͽ�����ǩ
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
	Input:          1non_end_mark��ʾҪѰ�ҵ�Ԫ���Ƿ�û�н�����ǣ�����<img.....>,���bool non_end_mark=true
					��ô��������<a..>...</a>�����ı�ǩ
	Output:         // �����������˵����
	Return:         //���û���ҵ�Ԫ�أ��򷵻�source.size()
					//�ҵ��򷵻���һԪ�ص���һλ��
					//����������ֵΪsource.size(),�򲢲���ȷ��Ԫ���Ƿ���ڣ�����
					//���element�Ƿ�Ϊ�����ж�Ԫ���Ƿ����
	Other:
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

/*************************************************************
*Function:get info of national country team
*input:source is page string
		s_nteam is a struct to store the useful info
*
*/
void CountryPage(const string& source, _sNTeam& s_nteam, const string&urlname);
void CountryPage(const string& source, _sNTeam& s_nteam);
//��ȡ��Ա��Ϣ
void PlayerPage(const string& source,_sPlayer& s_player);

//��ȡdate
void GetDate(_sDate& Date);

//���ж�ȡ�ַ���,�����н�������λ��
// prama:tableStrΪԭʼ�������ļ�
//		lineStrΪ���д�tableStr�ж�ȡ���ַ���
//other��������ж�ȡû�л��з��ˣ��ͷ���tableStr.size();
string::size_type ReadLine(const string& tableStr,string& lineStr,string::size_type pos=0);

string::size_type locate(string &line,const string& tag,int index);//��λ����index��tag��,��ǰ�б�����line�С��ҵ��򷵻�true
string::size_type locate(string &line,const string&tag);

string extract(string&line);//��ȡline�е�����

void eraseTag(string &source,char tag);//ɾ��source�е�����tag��ǩ

//��ȡ�������
//�����غõ���ҳ����ȡ������string��
std::string ExtractDataToStr(string& str);
std::string ExtractTableDataEx2(string& str);

//�ӱ����ļ��ж�ȡ���ݴ���pageSrc������ɹ�����true
bool ReadFromFile(string filePath,string& pageSrc);
bool WriteToFile(string filePath,string& pageSrc);

//��url����ȡ��country������player������
//non_end_markΪ��ʱ��Ҫ������׺����non_end_markΪ��ʱ��Ҫȥ����׺��
bool GetUrlName(const string& url,string&name,bool non_end_mark=false);


/** \brief �ó�����Ҫ��ͼ����ҳ�е�ͼƬ�����ӵ�ַ
 *
 * \param pageSrc ��ҳԴ�� 
 * \param tag_mark ��ǩ�����ڴ���ȡ��ͼƬ��ַ��ɸѡ,����ͼƬ��ַ�е�"flag"�ֶ�,"country_images"�ֶ�
 * \param imageUrl url of image
 * \return ����ҵ��򷵻�true�����򷵻�false
 * \other �����ı�ǩӦ���������ģ�����������ȡ�����ַʱ��miniflag����flag������flag������/flagȷʵ���еģ�
 * \����Ӧ����\flagΪ��ǩ
 */  
bool extractImageUrl(const string&pageSrc, const string& tag_mark, string &imageUrl);
bool getCoach(const string&pageSrc, _Coach& oCoach); //��ȡ������Ϣ
void walk_tree( tree<HTML::Node> const & dom );
void walk_tree( tree<HTML::Node>::iterator it,string & str );
