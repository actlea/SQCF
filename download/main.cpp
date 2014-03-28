#include "download.h"
#include "Table.h"
#include "StringPlus.h"
#include "unity.h"
#include "option.h"
#include "Data.h"

//#include <iomanip>

//****************************main*******************************************************

int main(int argc, char* argv[])
{

	//��ʼ���ʵ䣬���Ǳ���������
	if(InitDict() != 0)
		return -1;
	
	string pageSrc; //�洢���ص���ҳ
	string seedLink; //�����ص�����
	string QueLink; //���ڴ�ż�����������ӣ��������ǴӶ�����ȡ������
	//string::size_type i = 0, j=0;
	//string::size_type SrcLen = 0;	//Դ�ļ��ĳ���
	//string::size_tsype  Location=0;
	//string::size_type  Temp=0;
	//string::size_type  Linkend=0;
	//string nationalTeam = NFTPAGE;
	//string Link;//�洢һ�������ԡ�href=����ͷ���ԡ�</a>����β
	//string  LinkURL;//���ڴ洢��Link�з�������ĳ�����
	ParseURL _parseURL;//�����ֶκ�·����

	deque<string> LinkQ; //������Ӷ���
	set<string> LinkSet; //����Ѿ�������ĳ��˶��е�����
	set<string> AllLink; //����Ѿ�������Ļ���û�д�������Ǵ��ڶ����е�����
	map<string,URL>mPageLinks;//�洢��ҳ��������ص�����
	set<string> ErrorLink; //ר�Ŵ洢��ҳ����ʧ�ܵ�url��ַ

	_sPlayer sPlayer;
	_sNTeam sNTeam;
	vector<_sPlayer> vec_Player;
	vector<_sNTeam> vec_NTeam;

	//��ʼ�����Ӷ���
	seedLink = NFTURL; //NFT��ַ	
	LinkQ.push_back(seedLink);//����ʼ�������ӷ��뵽����LinkQ
	cout << seedLink <<" ����У�"<<endl;
	_parseURL = SplitURL(seedLink);//�����ֶκ�·��������

	ofstream onfile;
	onfile.open("./Country/tempLink.txt");

	ofstream errorUrl;
	onfile.open("./Country/errorUrl.txt");

	ofstream ofile; //�洢��ʱ���ص���ҳ
	string filename;

/*	
	//����������ӣ�����ֻ����
	while(LinkQ.size()!=0)
	{
		QueLink = LinkQ.front(); //��ȡ���Ӷ��е�ͷ���,��û�дӶ�����ɾ��
		
		if(QueLink.find(_parseURL.host)==string::npos)//�ж�QueLink�Ƿ��������ֶΣ�û�еĻ���Ҫ����
		{
			QueLink.insert(0,_parseURL.host);
		}
		cout << QueLink << " �����У�" << endl;
		AllLink.insert(QueLink);//AllLink�д�ŵ������е�����
		LinkQ.pop_front();		//ɾ��ͷ���

		if(!LinkSet.count(QueLink)) //�жϸ������Ƿ����
		{
			LinkSet.insert(QueLink); //���뵽�Ѿ���������ݼ�
			
			//���ȣ�������ҳ
			/////////////////////////////////////
			if((UrlFlag(QueLink))==PLAYER)
			{
				continue;
			}
			pageSrc = DownLoadPage(QueLink);
			//��ҳ����ʧ�ܣ���¼����ʧ�ܵ�url��ַ
			if(pageSrc.compare("error")==0)
			{
				//cout << QueLink << "...��������ʧ��!" << endl;
				if(!ErrorLink.count(QueLink))
				{
					ErrorLink.insert(QueLink);
				}
				pageSrc.clear();
				QueLink.clear();
				continue;
				//exit(-1);
			}
			//�ж���ҳ���ͣ������UTF-8,��ת��
			if(JudgeCode(pageSrc))
			{
				TransferCode(pageSrc);
			}
			//ȥ�����ӵ�ַ�еĻ��з�
			if(QueLink.find('\n')!=string::npos)
			{
				QueLink.erase(QueLink.find('\n'),1);
			}
			cout << "��ȡ" << QueLink << " �е�����" << endl;
		
			/*
			*��ȡcountry��continent�е����ӣ�����ȡcountry��ҳ�е���Ϣ
			*/
		//	mPageLinks = GetHyperLinks(pageSrc);//��ȡ��pageSrc�е��������õ���ҳ���ӣ��������˷���
		//	map<string,URL>::iterator m_iter;
		//	for(m_iter=mPageLinks.begin();m_iter!=mPageLinks.end();m_iter++)
		//	{
		/*		URL e = m_iter->second;
				string url=m_iter->first;
				//����player֮������Ӵ���AllLink��
				if(!AllLink.count(url)
					&& e!=PLAYER)
				{
					//�����COUNTRY���͵����ӣ���Ҫ��������ַ�,���� /country/10/2014/Armenia.html
					
					if(e==COUNTRY)
					{						
						_sDate Date;
						GetDate(Date);
						string strYear = itos(Date._year);
						string::size_type pos=0;
						pos = url.find_last_of('/');
						url.insert(pos,"/"+strYear);
					}
					AllLink.insert(url);
					LinkQ.push_back(url);					
				}/*end if*/
				
		/*	}//for
			//����COUNTRY�͵���ҳ����ȡ�����Ϣ
			if((UrlFlag(QueLink))==COUNTRY)
			{
				CountryPage(pageSrc,sNTeam);
				
				//�洢���ص���ҳ
				filename = "./Country/"+QueLink.substr(QueLink.find_last_of('/')+1,QueLink.find_last_of('.')-QueLink.find_last_of('/')-1);
				filename.append(".txt");
				ofile.open(filename);
				ofile<<pageSrc;
				ofile.flush();
				filename.clear();
			}
			
			QueLink.clear();
		    pageSrc.clear();
		}//end if(!LinkSet.count(QueLink))
		
	}//end while(LinQ.size()!=0)
	
	set<string>::iterator s_errorlink_iter;
	for(s_errorlink_iter=ErrorLink.begin();s_errorlink_iter!=ErrorLink.end();++s_errorlink_iter)
	{
		errorUrl<<(*s_errorlink_iter);
		errorUrl<<"\n";
	}
	errorUrl.close();
	
	//�洢map�е���������
	map<string,URL>::iterator m_iter;
	for(m_iter=mPageLinks.begin();m_iter!=mPageLinks.end();m_iter++)
	{
		onfile << m_iter->first + "\n" ;
	}//for
	onfile.close();
/*
	if(!InitCnameFile())
	{
		cout << "���Ҷ����Ƴ�ʼ��ʧ��!" << endl;
	}

	GetCountryTeamId(); //��ȡsina��ҳ�еĹ��Ҷ�id,�������ԣ���idû�й��ɿ�ѭ
*/

	//ExtractCountryCoach();//��ȡ���Ҷӽ�����Ϣ
		//��ȡ���
	//ExtractTableDataFromUrl(wikiURL, of_country_zh_en_name);
	//DoCountryInfo();

	/////////////////////////////////////////////////////////////////////////////
	//national football team��ҳ
	//nFteamURL = www.national-football-teams.com/continent/3/Asia.html

/////////////////////////Test///////////////////////////////////////////////////
	//string pageSrc;
#pragma region ��ȡcountryҳ����Ϣ
	//string url1 = "http://www.national-football-teams.com/country/5/2014/Andorra.html";
	//string url2 = "http://www.national-football-teams.com/player/362/Roman_Berezovsky.html";
	//string name1,name2;
	//GetUrlName(url1,name1,false);
	//GetUrlName(url2,name2,true);
	//if(ReadFromFile("./Country/Armenia.txt",pageSrc))
	//{
	//	CountryPage(pageSrc,sNTeam);
	//}
#pragma endregion
#pragma  region ��ȡplayerҳ����Ϣ
	//string playerurl = "www.national-football-teams.com/player/48967/Emerson_Cesario.html";
	//pageSrc = DownLoadPage(playerurl);
	//ofstream out;
	//out.open("./Country/Emerson_Cesario.txt");
	//out << pageSrc;
	//out.close();
	if(ReadFromFile("./Country/Emerson_Cesario.txt",pageSrc))
		PlayerPage(pageSrc,sPlayer);
#pragma endregion


//Parse some html code
//string html = "<html><body>hey</body></html>";
//HTML::ParserDom parser;
//tree<HTML::Node> dom = parser.parseTree(html);
//
////Print whole DOM tree
//cout << dom << endl;
//
////Dump all links in the tree
//tree<HTML::Node>::iterator it = dom.begin();
//tree<HTML::Node>::iterator end = dom.end();
//for (; it != end; ++it)
//{
//	if (strcmp(it->tagName().c_str(), "A") == 0)
//	{
//		it->parseAttributes();
//		cout << it->attribute("href").second << endl;
//	}
//}
//
////Dump all text of the document
//it = dom.begin();
//end = dom.end();
//for (; it != end; ++it)
//{
//	if ((!it->isTag()) && (!it->isComment()))
//	{
//		cout << it->text();
//	}
//}
//cout << endl;
	
	return 0;
}

