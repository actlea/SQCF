// download.cpp : �������̨Ӧ�ó������ڵ㡣
//
//���г��������Դ�ļ����ڵ�ǰĿ¼��
//���г����������ļ����洢��D:\\zjm\\nation_fb_info\\Ŀ¼��
#include "download.h"

SOCKET sockClient;
SOCKADDR_IN addrSrv; //��ű��ص�ַ��Ϣ



//********************************DownLoadPage*****************************
//LinkURLȥ��������http:// ��ftp://֮���Э��ͷ
string DownLoadPage(string LinkURL)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	struct ParseURL parseURL;
	struct hostent *hop;
	struct in_addr ipAddr;

	//�Ȳ鿴�Ƿ���url <scheme>�ֶ�
	if(LinkURL.find("http")!=string::npos ||
		LinkURL.find("ftp")!=string::npos)
	{
		cout << "Ҫȥ��Э��ͷ�ֶ�"  << endl;
	}

	string s = "error";
	string PageSrc;  //�洢���ص���ҳ

	cout << "��ǰ��������: " << LinkURL << endl;
	wVersionRequested = MAKEWORD(1,1);
	err = WSAStartup(wVersionRequested,&wsaData); //�ú����Ĺ����Ҽ���һ��winsocekt��汾
	if(err != 0)
	{
		cout << "�����׽��ִ���! \n";
		return s;
	}

	if(LOBYTE(wsaData.wVersion) != 1 ||
		 HIBYTE( wsaData.wVersion ) != 1 )
	{
		WSACleanup();
		return s;
	}

	parseURL = SplitURL(LinkURL);

	//��������������ȡ������ip��ַ
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

	//�����׽���
	sockClient=socket(AF_INET,SOCK_STREAM,0);//�������ӵĿɿ�����SOCK_STREAM
	addrSrv.sin_addr.S_un.S_addr=inet_addr(inet_ntoa(ipAddr));//�������ֽ���long��ת��Ϊ�����ֽ���
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(80); //htos�������˿�ת�����ַ�

	//���ӷ�����
    if(!ConnectSever())
	{
		if(!ConnectSever())
		{
			cout<<"���ӳ�ʱ������"<<endl;
		   /* system("pause");*/
			return s;
		}
	}

	//����http����
	string Request_1="GET /";
	string Request_2=" HTTP/1.0\r\n";
	string Request_3="Host:";
	string Request_4="\r\nConnection:Close\r\n\r\n";
	size_t ReLen=parseURL.ReSufix.length();


	string Request;	
	Request=Request_1+parseURL.ReSufix+Request_2+Request_3+parseURL.host+Request_4;

	char const *RequestP=Request.c_str();
	string::size_type  TempByte=0;

	//����http����
	try
   {
	   //���׽����趨Ϊ������ʽ
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

	//���շ��������ص�����
	string::size_type  ByteReadCount=0;
	int timeOut=10000;
    if(SOCKET_ERROR==setsockopt(sockClient,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeOut,sizeof(int)))
		return s;
/////////////////////////////////////////////////////////////////////////////////////////////////////
	//��ʾ���ص���Ϣ������ע��
	
	while(TempByte>0)
	{
		//cout<<"�������ԣ�"<<parseURL.host+parseURL.ReSufix<<"**********������"<<endl;
		TempByte=recv(sockClient,PageBuf+ByteReadCount,PAGE_BUF_SIZE-ByteReadCount,0);
		if(TempByte==4294967295)
			//system("pause");
			break;
		if(TempByte>0)
		{
			ByteReadCount+=TempByte;
		//	cout<<"�����ˣ�"<<TempByte<<"�ֽڵ����ݣ�"<<endl;
		}
	}
	PageBuf[ByteReadCount]='\0';
	PageSrc.append(PageBuf);

//////////////////////////////////////////////////////////////////////////////////////////////
	//�����ļ��Ƿ���ȷ����
	/*
	ofstream of;
	//LinkURL.append(".txt");
	of.open("d:\\htmlPage.txt");
	of<<PageBuf<<endl;
	of.close();
	*/

	free(PageBuf);
	//�ر��׽��ֿ�
	closesocket(sockClient);
	WSACleanup();

	cout<<"����  "<<LinkURL<<"  �ɹ�!"<<endl;
	return PageSrc;
}

//��url�е�������·���ֶηֿ�
struct ParseURL SplitURL(string URL)
{
	string::size_type i;
	string::size_type  UrlLen=URL.length();
	struct ParseURL parseURL;
	//http://www.cnblogs.com/tornadomeet/archive/2012/04/11/2442140.html
	//�ҵ�·��ǰ���ֶ�
	for(i=0;i<UrlLen;i++)
	{
		if(URL[i]=='/')
			break;
	}
	//���û��·���ֶ�
	if(i==UrlLen)
	{
		parseURL.host=URL;
		return parseURL;
	}
	//��·���ֶμ�������ַ��洢
	char *ReSufix=(char*)malloc((UrlLen-i)*sizeof(char));
	int j;
	for(j=0;j+i+1<UrlLen;j++)
	{
		ReSufix[j]=URL[j+i+1];
	}
	ReSufix[j]='\0';

	//��ȡURL��·���ֶ�ǰ���ַ�
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
	//�趨Ϊ��������ʽ
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
//�趨��ʱ��ʱ��
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
//�ж���ҳ�ĸ�ʽ
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
//����ת��
void TransferCode(string &s)
{
	//��utf-8ת��Ϊ���ֽ���Ҫ�ĳ���
	int nLen=MultiByteToWideChar(CP_UTF8,0,(LPCSTR)s.c_str(),s.size()+1,NULL,0);
	WCHAR*lpszW=NULL;
	
	//�����µĴ洢�ռ������洢ת���������
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

	//��utf-8ת��Ϊ���ֽ�
	int nRtn=MultiByteToWideChar(CP_UTF8,0,(LPCSTR)s.c_str(),s.size()+1,lpszW,nLen);

	//�õ���Ҫת�����ֽ���
	int needBytes=WideCharToMultiByte(	CP_ACP,0,(LPCWSTR)lpszW,-1,NULL,0,NULL,NULL);
	char* results=NULL;

    //�����µĴ洢�ռ������洢ת���������
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

	//���б���ת��
	WideCharToMultiByte(CP_ACP,0,(LPCWSTR)lpszW,-1,results,needBytes,NULL,NULL);
	s=results;

	delete[] lpszW;
	delete[] results;
}
