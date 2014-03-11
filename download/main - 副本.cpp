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

	//��ʼ�����Ӷ���
	seedLink = NFTURL; //NFT��ַ	
	LinkQ.push_back(seedLink);//����ʼ�������ӷ��뵽����LinkQ
	cout << seedLink <<" ����У�"<<endl;
	_parseURL = SplitURL(seedLink);//�����ֶκ�·��������

	ofstream onfile;
	onfile.open("tempLink.txt",ios::app);

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
			//PLAYER����ҳ��������
			if((UrlFlag(QueLink))==PLAYER)
			{
				continue;
			}
			
			pageSrc = DownLoadPage(QueLink);
			if(pageSrc.compare("error")==0)
			{
				cout << QueLink << "...��������ʧ��!" << endl;
				pageSrc.clear();
				QueLink.clear();
				exit(-1);
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
			mPageLinks = GetHyperLinks(pageSrc);//��ȡ��pageSrc�е��������õ���ҳ���ӣ��������˷���
			map<string,URL>::iterator m_iter;
			for(m_iter=mPageLinks.begin();m_iter!=mPageLinks.end();m_iter++)//mPageLinks�е�player����Ҫ��������
			{
				URL e = m_iter->second;
				//ֻ����player��AllLink��
				if(!AllLink.count(m_iter->first)
					&&(e!=PLAYER))
				{
					AllLink.insert(m_iter->first);
					LinkQ.push_back(m_iter->first);				
				}
				onfile << m_iter->first +"\n";
				onfile.flush();
			}//for
			//����COUNTRY�͵���ҳ����ȡ�����Ϣ
			if((UrlFlag(QueLink))==COUNTRY)
			{
			}
			QueLink.clear();
		    pageSrc.clear();
		}//end if(!LinkSet.count(QueLink))
		
	}//end while(LinQ.size()!=0)
	
	onfile.close();
	//map<string,URL>::iterator m_iter;
	//for(m_iter=mPageLinks.begin();m_iter!=mPageLinks.end();m_iter++)
	//{
	//	onfile << m_iter->first + "\n" ;
	//}//for
	//onfile.close();
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
	return 0;
}

