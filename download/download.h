#pragma once
#include "stdafx.h"


#pragma comment(lib,"ws2_32.lib")

using namespace std;
#define PAGE_BUF_SIZE 1048576

//�洢�������ӵķ����������ֶκ�·���ֶ�
struct ParseURL
{
	string host;
	string ReSufix;
};
//ȫ�ֱ�������
extern SOCKET sockClient;
extern SOCKADDR_IN addrSrv; //��ű��ص�ַ��Ϣ

//////////////////////////////////////////////////////////////////////////////////////////////////
//����
#pragma region �����ĺ���
//****************************��ָ��������������ҳ********************************************
string DownLoadPage(string LinkURL); //ͨ����������Դ�ļ�
struct ParseURL SplitURL(string URL);//������һ��URL����������ֶκ�·���ֶ�

bool setTimeOut(const int seconds,const int microSeconds);//���ó�ʱʱ��,���ӷ�������ʱ��ʹ��

bool setSocketMode(const unsigned short modeType);//�趨�׽��ַ�ʽ�������������

bool ConnectSever();//���ӷ�����

bool JudgeCode(string s); //�ж���ҳ�ĸ�ʽ

void TransferCode(string &s);


#pragma endregion

//////////////////////////////////////////////////////////////////////////////////////////////////

