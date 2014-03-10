#include "download.h"
#include "Table.h"
#include "StringPlus.h"
#include "unity.h"
#include "option.h"
//#include <iomanip>

//****************************main*******************************************************

int main(int argc, char* argv[])
{

	//初始化词典，就是表格的属性列
	if(InitDict() != 0)
		return -1;
	
	string pageSrc; //存储下载的网页
	string seedLink; //待下载的链接
	string QueLink; //用于存放即将处理的链接，该链接是从队列中取出来的
	string::size_type i = 0, j=0;
	string::size_type SrcLen = 0;	//源文件的长度
	string::size_type  Location=0;
	string::size_type  Temp=0;
	string::size_type  Linkend=0;
	string nationalTeam = NFTPAGE;
	string Link;//存储一个链接以“href=”开头，以“</a>”结尾
	string  LinkURL;//用于存储从Link中分离出来的超链接
	ParseURL _parseURL;//主机字段和路径名

	deque<string> LinkQ; //存放链接队列
	set<string> LinkSet; //存放已经处理过的出了队列的链接
	set<string> AllLink; //存放已经处理过的或者没有处理过但是存在队列中的链接

	//初始化种子队列
	seedLink = NFTURL; //NFT网址	
	LinkQ.push_back(seedLink);//将初始种子链接放入到队列LinkQ
	cout << seedLink <<" 入队列！"<<endl;
	_parseURL = SplitURL(seedLink);//主机字段和路径名分离

	//处理队列种子
	while(LinkQ.size()!=0)
	{
		QueLink = LinkQ.front(); //获取链接队列的头结点,但没有从队列中删除
		
		if(QueLink.find(_parseURL.host)==string::npos)//判断QueLink是否含有主机字段，没有的话，要加上
		{
			QueLink.insert(0,_parseURL.host);
		}
		cout << QueLink << " 出队列！" << endl;
		AllLink.insert(QueLink);//AllLink中存放的是所有的链接
		LinkQ.pop_front();		//删除头结点

		if(!LinkSet.count(QueLink)) //判断该链接是否处理过
		{
			LinkSet.insert(QueLink); //加入到已经处理的数据集
			
			//首先，下载网页
			pageSrc = DownLoadPage(QueLink);
			if(pageSrc.compare("error")==0)
			{
				cout << QueLink << "...链接下载失败!" << endl;
				pageSrc.clear();
				QueLink.clear();
				exit(-1);
			}
			//判断网页类型，如果是UTF-8,则转码
			if(JudgeCode(pageSrc))
			{
				TransferCode(pageSrc);
			}
			//去除链接地址中的换行符
			if(QueLink.find('\n')!=string::npos)
			{
				QueLink.erase(QueLink.find('\n'),1);
			}

			//对下载的源文件进行处理
			if(UrlFlag(QueLink)==HOME)
			{//如果是主页，则下载continent链接
				
			}

		}//end if(!LinkSet.count(QueLink))
	}//end while(LinQ.size()!=0)
	

/*
	if(!InitCnameFile())
	{
		cout << "国家队名称初始化失败!" << endl;
	}

	GetCountryTeamId(); //提取sina网页中的国家队id,经过测试，该id没有规律可循
*/

	//ExtractCountryCoach();//提取国家队教练信息
		//提取表格
	//ExtractTableDataFromUrl(wikiURL, of_country_zh_en_name);
	//DoCountryInfo();

	/////////////////////////////////////////////////////////////////////////////
	//national football team网页
	//nFteamURL = www.national-football-teams.com/continent/3/Asia.html
	return 0;
}

