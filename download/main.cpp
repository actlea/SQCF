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
	string pageSrc; //�洢���ص���ҳ
	string seedLink; //�����ص�����
	string QueLink; //���ڴ�ż�����������ӣ��������ǴӶ�����ȡ������
	string::size_type i = 0, j=0;
	string::size_type SrcLen = 0;	//Դ�ļ��ĳ���
	string::size_type  Location=0;
	string::size_type  Temp=0;
	string::size_type  Linkend=0;
	string Link;//�洢һ�������ԡ�href="http://����ͷ���ԡ�</a>����β
	string  LinkURL;//���ڴ洢��Link�з�������ĳ�����
	ParseURL _parseURL;//�����ֶκ�·����

	deque<string> LinkQ; //������Ӷ���
	set<string> LinkSet; //����Ѿ�������ĳ��˶��е�����
	set<string> AllLink; //����Ѿ�������Ļ���û�д�������Ǵ��ڶ����е�����

	
	//��ʼ���ʵ䣬���Ǳ���������
	//if(InitDict() != 0)
	//	return -1;
	
	//��ȡ���
	//ExtractTableDataFromUrl(wikiURL, of_country_zh_en_name);
	//DoCountryInfo();

	/////////////////////////////////////////////////////////////////////////////
	//national football team��ҳ
	//nFteamURL = www.national-football-teams.com/continent/3/Asia.html
	
	//��ʼ�����Ӷ���
	seedLink = nFteamURL;
	
	//����ʼ�������ӷ��뵽����LinkQ
	LinkQ.push_back(seedLink);
	cout << seedLink <<" ����У�"<<endl;
	_parseURL = SplitURL(seedLink);//�����ֶκ�·��������

	//�����������
	while(LinkQ.size()!=0)
	{
	QueLink = LinkQ.front(); //��ȡ���Ӷ��е�ͷ���,��û�дӶ�����ɾ��
	//�ж�QueLink�Ƿ��������ֶΣ�û�еĻ���Ҫ����
	if(QueLink.find(_parseURL.host)==string::npos)
	{
		QueLink.insert(0,_parseURL.host);
	}
	cout << QueLink << " �����У�" << endl;
	AllLink.insert(QueLink); //AllLink�д�ŵ������е����ӣ��������δ�������
	LinkQ.pop_front();		//ɾ��ͷ���

	if(!LinkSet.count(QueLink)) //�жϸ������Ƿ����
	{
		LinkSet.insert(QueLink); //���뵽�Ѿ���������ݼ�
			
		pageSrc = DownLoadPage(QueLink);
		if(pageSrc.compare("error")==0)
		{
			cout << seedLink << "...��������ʧ��!" << endl;
			pageSrc.clear();
			QueLink.clear();
			exit(-1);
		}
		SrcLen = pageSrc.length();

		string::size_type n = 0;
		//�����UTF-8,��ôת��
		if(JudgeCode(pageSrc))
		{
			TransferCode(pageSrc);
		}

		//ȥ�����ӵ�ַ�еĻ��з�
		if(QueLink.find('\n')!=string::npos)
		{
			QueLink.erase(QueLink.find('\n'),1);
		}
		//��seedLink��Դ�ļ����д���
		ofstream onfile;
		onfile.open(nationalTeam);

		while(i<SrcLen)
		{
			//cout << "��ȡ" << QueLink << " �е����� " << endl;
			/////////////////////////////////////////////////////////////////////////////
			Location = pageSrc.find("<a",Temp); //Ѱ�����ӵĿ�ͷλ��
			if(Location==string::npos||Location+2>SrcLen)
			{
				break;
			}
			//Location += 2;
			j = Location;//�ҵ�ָ���ַ�����λ��
			Linkend=pageSrc.find("</a>",Location);//Ѱ�Ҷ�Ӧ���ӵĽ���λ��
			if(Linkend==string::npos||(Linkend+3>SrcLen))//����
			{
				break;
			}

			//�������е����ݿ�����Link��
			Linkend += (strlen("</a>")) - 1;
			while(j<=Linkend)
			{
				Link += pageSrc[j++];
			}
			//�жϸ������Ƿ���������Ҫ������
			//����ǣ���Link����ȡ�����Ӵ��������
			if(JudgeFootballInfo(Link))
			{
				cout << "��ȡ" << QueLink << " �е����� " << endl;
				LinkURL=SelectURL(Link);
				//��������ӻ�û�д����������
				if(AllLink.count(LinkURL)==0)
				{
					LinkQ.push_back(LinkURL);
					AllLink.insert(LinkURL);
					cout<<LinkURL<<"  �����У�"<<endl;
				}
				//TODO:�˴���Ӳ���
				//�ж����ӵ����ͣ�Ȼ����в�ͬ�Ĳ���
				//�����continent����ô���룬����country����
				//�����country,��ô���룬����player���Ӳ���ȡcountry��Ϣ
				//�����player,��ô��ȡplayer��Ϣ
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
		QueLink.clear(); //�������
		pageSrc.clear(); //��ҳԴ�ļ����
		onfile.close();
    }//end if(LinkSet.count(QueLink)) 
	}//end while
	*/
	return 0;
}

