#include "download.h"
#include "Table.h"
#include "StringPlus.h"
#include "unity.h"
#include "option.h"
#include <iomanip>

//****************************main*******************************************************

int main(int argc, char* argv[])
{

	/*
	string pageSrc; //存储下载的网页
	string seedLink; //待下载的链接
	string QueLink; //用于存放即将处理的链接，该链接是从队列中取出来的
	string::size_type i = 0, j=0;
	string::size_type SrcLen = 0;	//源文件的长度
	string::size_type  Location=0;
	string::size_type  Temp=0;
	string::size_type  Linkend=0;
	string Link;//存储一个链接以“href="http://”开头，以“</a>”结尾
	string  LinkURL;//用于存储从Link中分离出来的超链接
	ParseURL _parseURL;//主机字段和路径名

	deque<string> LinkQ; //存放链接队列
	set<string> LinkSet; //存放已经处理过的出了队列的链接
	set<string> AllLink; //存放已经处理过的或者没有处理过但是存在队列中的链接

	
	//初始化词典，就是表格的属性列
	//if(InitDict() != 0)
	//	return -1;
	
	//提取表格
	//ExtractTableDataFromUrl(wikiURL, of_country_zh_en_name);
	//DoCountryInfo();

	/////////////////////////////////////////////////////////////////////////////
	//national football team网页
	//nFteamURL = www.national-football-teams.com/continent/3/Asia.html
	
	//初始化种子队列
	seedLink = nFteamURL;
	
	//将初始种子链接放入到队列LinkQ
	LinkQ.push_back(seedLink);
	cout << seedLink <<" 入队列！"<<endl;
	_parseURL = SplitURL(seedLink);//主机字段和路径名分离

	//处理队列种子
	while(LinkQ.size()!=0)
	{
	QueLink = LinkQ.front(); //获取链接队列的头结点,但没有从队列中删除
	//判断QueLink是否含有主机字段，没有的话，要加上
	if(QueLink.find(_parseURL.host)==string::npos)
	{
		QueLink.insert(0,_parseURL.host);
	}
	cout << QueLink << " 出队列！" << endl;
	AllLink.insert(QueLink); //AllLink中存放的是所有的链接，处理过或未处理过的
	LinkQ.pop_front();		//删除头结点

	if(!LinkSet.count(QueLink)) //判断该链接是否处理过
	{
		LinkSet.insert(QueLink); //加入到已经处理的数据集
			
		pageSrc = DownLoadPage(QueLink);
		if(pageSrc.compare("error")==0)
		{
			cout << seedLink << "...链接下载失败!" << endl;
			pageSrc.clear();
			QueLink.clear();
			exit(-1);
		}
		SrcLen = pageSrc.length();

		string::size_type n = 0;
		//如果是UTF-8,那么转码
		if(JudgeCode(pageSrc))
		{
			TransferCode(pageSrc);
		}

		//去除链接地址中的换行符
		if(QueLink.find('\n')!=string::npos)
		{
			QueLink.erase(QueLink.find('\n'),1);
		}
		//对seedLink的源文件进行处理
		ofstream onfile;
		onfile.open(nationalTeam);

		while(i<SrcLen)
		{
			//cout << "提取" << QueLink << " 中的链接 " << endl;
			/////////////////////////////////////////////////////////////////////////////
			Location = pageSrc.find("<a",Temp); //寻找链接的开头位置
			if(Location==string::npos||Location+2>SrcLen)
			{
				break;
			}
			//Location += 2;
			j = Location;//找到指定字符串的位置
			Linkend=pageSrc.find("</a>",Location);//寻找对应链接的结束位置
			if(Linkend==string::npos||(Linkend+3>SrcLen))//超出
			{
				break;
			}

			//把链接中的内容拷贝到Link中
			Linkend += (strlen("</a>")) - 1;
			while(j<=Linkend)
			{
				Link += pageSrc[j++];
			}
			//判断该链接是否是我们需要的链接
			//如果是，从Link中提取出链接存入队列中
			if(JudgeFootballInfo(Link))
			{
				cout << "提取" << QueLink << " 中的链接 " << endl;
				LinkURL=SelectURL(Link);
				//如果该链接还没有处理过，则处理
				if(AllLink.count(LinkURL)==0)
				{
					LinkQ.push_back(LinkURL);
					AllLink.insert(LinkURL);
					cout<<LinkURL<<"  进队列！"<<endl;
				}
				//TODO:此处添加操作
				//判断链接的类型，然后进行不同的操作
				//如果是continent，那么进入，下载country链接
				//如果是country,那么进入，下载player链接并提取country信息
				//如果是player,那么提取player信息
				//onfile << LinkURL + "\n" ;
			 }// end if
			Temp = Linkend;
			i=j;
			j=0;
			Linkend=0;
			Link.clear();
		}// end while(i<SrcLen)
		i=0;
		Temp=0;
		QueLink.clear(); //链接清空
		pageSrc.clear(); //网页源文件清空
		onfile.close();
    }//end if(LinkSet.count(QueLink)) 
	}//end while
	*/
	return 0;
}

