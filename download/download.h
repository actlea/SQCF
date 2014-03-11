#pragma once
#include "stdafx.h"


#pragma comment(lib,"ws2_32.lib")

using namespace std;
#define PAGE_BUF_SIZE 1048576

//存储请求链接的服务器域名字段和路径字段
struct ParseURL
{
	string host;
	string ReSufix;
};
//全局变量声明
extern SOCKET sockClient;
extern SOCKADDR_IN addrSrv; //存放本地地址信息

//////////////////////////////////////////////////////////////////////////////////////////////////
//函数
#pragma region 公共的函数
//****************************从指定的链接下载网页********************************************
string DownLoadPage(string LinkURL); //通过链接下载源文件
struct ParseURL SplitURL(string URL);//把任意一个URL分离成主机字段和路径字段

bool setTimeOut(const int seconds,const int microSeconds);//设置超时时间,连接服务器的时候使用

bool setSocketMode(const unsigned short modeType);//设定套接字方式，阻塞或非阻塞

bool ConnectSever();//连接服务器

bool JudgeCode(string s); //判断网页的格式

void TransferCode(string &s);


#pragma endregion

//////////////////////////////////////////////////////////////////////////////////////////////////

