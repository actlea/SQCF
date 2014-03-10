// download.cpp : 定义控制台应用程序的入口点。
//
//所有程序操作的源文件都在当前目录下
//所有程序操作后的文件都存储在D:\\zjm\\nation_fb_info\\目录下
#include "download.h"

SOCKET sockClient;
SOCKADDR_IN addrSrv; //存放本地地址信息



//********************************DownLoadPage*****************************
//LinkURL去除了诸如http:// ，ftp://之类的协议头
string DownLoadPage(string LinkURL)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	struct ParseURL parseURL;
	struct hostent *hop;
	struct in_addr ipAddr;

	//先查看是否含有url <scheme>字段
	if(LinkURL.find("http")!=string::npos ||
		LinkURL.find("ftp")!=string::npos)
	{
		cout << "要去掉协议头字段"  << endl;
	}

	string s = "error";
	string PageSrc;  //存储下载的网页

	cout << "当前正在下载: " << LinkURL << endl;
	wVersionRequested = MAKEWORD(1,1);
	err = WSAStartup(wVersionRequested,&wsaData); //该函数的功能室加载一个winsocekt库版本
	if(err != 0)
	{
		cout << "加载套接字错误! \n";
		return s;
	}

	if(LOBYTE(wsaData.wVersion) != 1 ||
		 HIBYTE( wsaData.wVersion ) != 1 )
	{
		WSACleanup();
		return s;
	}

	parseURL = SplitURL(LinkURL);

	//根据主机域名获取主机的ip地址
	const char *host = parseURL.host.c_str();
	hop = gethostbyname(host);

	if(hop==NULL)
	{
		cout<<"gethostbyname() error in GetIpByHost:"<<parseURL.host<<endl;
		//system("pause");
		return s;
	}
		
	LPSTR lpAddr=hop->h_addr;
	memmove(&ipAddr,lpAddr,4);

	//创建套接字
	sockClient=socket(AF_INET,SOCK_STREAM,0);//面向连接的可靠服务SOCK_STREAM
	addrSrv.sin_addr.S_un.S_addr=inet_addr(inet_ntoa(ipAddr));//将主机字节序long型转换为网络字节序
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(80); //htos用来将端口转换成字符

	//连接服务器
    if(!ConnectSever())
	{
		if(!ConnectSever())
		{
			cout<<"连接超时！！！"<<endl;
		   /* system("pause");*/
			return s;
		}
	}

	//发送http请求
	string Request_1="GET /";
	string Request_2=" HTTP/1.0\r\n";
	string Request_3="Host:";
	string Request_4="\r\nConnection:Close\r\n\r\n";
	size_t ReLen=parseURL.ReSufix.length();


	string Request;	
	Request=Request_1+parseURL.ReSufix+Request_2+Request_3+parseURL.host+Request_4;

	char const *RequestP=Request.c_str();
	string::size_type  TempByte=0;

	//发送http请求
	try
   {
	   //将套接字设定为阻塞方式
       if(!setSocketMode(0))
		   throw runtime_error("ioctlsocket error... ");
   }
   catch(const runtime_error& e)
   {
	   cerr << "error: " << e.what(); 
	   return s;
  }


    TempByte=send(sockClient,RequestP,Request.length(),0);
	char *PageBuf=(char*)malloc(PAGE_BUF_SIZE);
	memset(PageBuf,0,PAGE_BUF_SIZE);

	//接收服务器返回的数据
	string::size_type  ByteReadCount=0;
	int timeOut=10000;
    if(SOCKET_ERROR==setsockopt(sockClient,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeOut,sizeof(int)))
		return s;
/////////////////////////////////////////////////////////////////////////////////////////////////////
	//显示下载的信息，可以注释
	
	while(TempByte>0)
	{
		//cout<<"接受来自："<<parseURL.host+parseURL.ReSufix<<"**********的数据"<<endl;
		TempByte=recv(sockClient,PageBuf+ByteReadCount,PAGE_BUF_SIZE-ByteReadCount,0);
		if(TempByte==4294967295)
			//system("pause");
			break;
		if(TempByte>0)
		{
			ByteReadCount+=TempByte;
		//	cout<<"接收了："<<TempByte<<"字节的数据！"<<endl;
		}
	}
	PageBuf[ByteReadCount]='\0';
	PageSrc.append(PageBuf);

//////////////////////////////////////////////////////////////////////////////////////////////
	//测试文件是否正确下载
	/*
	ofstream of;
	//LinkURL.append(".txt");
	of.open("d:\\htmlPage.txt");
	of<<PageBuf<<endl;
	of.close();
	*/

	free(PageBuf);
	//关闭套接字库
	closesocket(sockClient);
	WSACleanup();

	cout<<"下载  "<<LinkURL<<"  成功!"<<endl;
	return PageSrc;
}

//将url中的主机和路劲字段分开
struct ParseURL SplitURL(string URL)
{
	string::size_type i;
	string::size_type  UrlLen=URL.length();
	struct ParseURL parseURL;
	//http://www.cnblogs.com/tornadomeet/archive/2012/04/11/2442140.html
	//找到路径前的字段
	for(i=0;i<UrlLen;i++)
	{
		if(URL[i]=='/')
			break;
	}
	//如果没有路径字段
	if(i==UrlLen)
	{
		parseURL.host=URL;
		return parseURL;
	}
	//将路径字段及后面的字符存储
	char *ReSufix=(char*)malloc((UrlLen-i)*sizeof(char));
	int j;
	for(j=0;j+i+1<UrlLen;j++)
	{
		ReSufix[j]=URL[j+i+1];
	}
	ReSufix[j]='\0';

	//提取URL中路径字段前的字符
	char *exChange=(char*)malloc(sizeof(char)*(i+1));
	strncpy(exChange,URL.c_str(),i);
	exChange[i]='\0';

	parseURL.host.append(exChange);
	parseURL.ReSufix.append(ReSufix);

	free(ReSufix);
	free(exChange);

	return parseURL;
}

bool ConnectSever()
{
	//设定为非阻塞方式
	try
	{
		if(!setSocketMode(1))
			throw runtime_error("ioctlsocket error");
	}
     catch(const runtime_error& e)
	 {
		 cerr<<"error:"<<e.what();
		 return false;
	 }
	connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
	if(!setTimeOut(10,0))
		return false;
		return true;           
}

bool setSocketMode(const unsigned short modeType)
{
	int returnCode=ioctlsocket(sockClient,FIONBIO,(unsigned long*)&modeType);
   if(SOCKET_ERROR==returnCode)
	   return false;
   return true;
}
//设定超时的时间
bool setTimeOut(const int seconds,const int microSeconds)
{
	timeval timeOut;
	fd_set readFd;

	FD_ZERO(&readFd);
	FD_SET(sockClient,&readFd);

	timeOut.tv_sec=seconds;
	timeOut.tv_usec=microSeconds;

	int returnCode=select(sockClient+1,0,&readFd,0,&timeOut);
    if(returnCode<=0)
            return false;
	return true;
}

//***************************************JudgeCode(string s)***********************************
//判断网页的格式
bool JudgeCode(string s)
{
	string::size_type Loc = 0;
	Loc=s.find("charset=");
	string cs;
	if(Loc==string::npos)
		return false;
	else
	{
		string::size_type  end=s.find(">",Loc);
		cs=s.substr(Loc,end-Loc);
	    if(cs.find("utf-8")!=string::npos||cs.find("UTF-8")!=string::npos)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

}

//**********************************************TransferCode(string &s)************************
//编码转换
void TransferCode(string &s)
{
	//将utf-8转换为宽字节需要的长度
	int nLen=MultiByteToWideChar(CP_UTF8,0,(LPCSTR)s.c_str(),s.size()+1,NULL,0);
	WCHAR*lpszW=NULL;
	
	//申请新的存储空间用来存储转换后的内容
	try
	{
		lpszW=new WCHAR[nLen];
		if(!lpszW)
			throw bad_alloc("utf8 to widechar memory bad alloc...");
		memset(lpszW,'\0',nLen+1);
	}
	catch(const bad_alloc& e)
	{
		cerr<<e.what();
		return ;
	}

	//将utf-8转换为宽字节
	int nRtn=MultiByteToWideChar(CP_UTF8,0,(LPCSTR)s.c_str(),s.size()+1,lpszW,nLen);

	//得到需要转换的字节数
	int needBytes=WideCharToMultiByte(	CP_ACP,0,(LPCWSTR)lpszW,-1,NULL,0,NULL,NULL);
	char* results=NULL;

    //申请新的存储空间用来存储转换后的内容
	try
	{
		results=new char[needBytes+1];
		if(!results)
			throw bad_alloc("widechar to ansi memory bad alloc...");
		memset(results,'\0',needBytes+1);
	}
	catch(const bad_alloc& e)
	{
		cerr<<e.what();
		return;
	}

	//进行编码转换
	WideCharToMultiByte(CP_ACP,0,(LPCWSTR)lpszW,-1,results,needBytes,NULL,NULL);
	s=results;

	delete[] lpszW;
	delete[] results;
}
