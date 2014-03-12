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
	//string::size_type  Location=0;
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
	if(ReadFromFile("./Country/Armenia.txt",pageSrc))
	{
		CountryPage(pageSrc,sNTeam);
	}
	
	return 0;
}

