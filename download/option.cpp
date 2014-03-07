#include "option.h"

using namespace std;

std::set<string> CTKey;
/**
*sourceΪԴ�ַ��� bTagΪ��ʼ��ǩ eTagΪ������ǩ 
*posΪ0ʱ�����ص��ַ�������ǩ����
*posΪ1ʱ�����ص��ַ���������ǩ���֣�Ĭ��pos=0
*@����source��bTag��eTag֮����ַ�
*/
string GetTag(const string& source, const string& bTag, const string& eTag,int pos)
{
	string sContent;
	string::size_type adr1=0,adr2=0;

	adr1 = source.find(bTag,pos);
	adr2 = source.find(eTag,adr1+bTag.length()+1);
	if(adr1==string::npos
		|| adr2 ==string::npos)
	{
		cout << "�����б�ǩ" << endl;
		return sContent;
	}
	if(pos==0)
	{
		sContent =source.substr(adr1,adr2-adr1);
		sContent.append(eTag);
	}
	else
	{
		sContent =source.substr(adr1+bTag.length(),adr2-adr1-bTag.length());
	}
	
	return sContent;

}

//�ж��Ƿ���������Ҫ������
//һ��LinkText����ʽΪ<a ...>...</a>
bool JudgeFootballInfo(const string& LinkText)
{
	//�鿴href�е���Ϣ
	string hrefText = GetTag(LinkText,"href=\"","\"");
	if(hrefText.find("country")!=string::npos
		||hrefText.find("continent")!=string::npos)
	{
		return true;
	}
	return false;
}

//�Ӵ������ӵ�ê�ı�����ȡ��������
string SelectURL(const string& AnchorText)
{
	string LinkURL;
	LinkURL = GetTag(AnchorText,"href=\"","\"",1); //��������ǩ����	
	return LinkURL;
}

//��wiki����ȡ��Ҫ�ı������
void DoCountryInfo()
{
	ofstream onfile;
	ifstream infile;

	onfile.open(nf_country_zh_en_name);
	infile.open(of_country_zh_en_name);

	if(!onfile || !infile)
	{
		cout << "file open error" << endl;
		return;
	}

	string linestr; //���ж�ȡ
	string tabSrc;	//�������
	string tmpStr;
	while(getline(infile,linestr))
		tabSrc += linestr + "\n";

	//ȥ������<a
	while (tabSrc.find("<a") != string::npos)
	{
		DeleteTag(tabSrc, "a", tmpStr);
		tabSrc = tmpStr;
	}
	//ȥ��img
	if(tabSrc.find("<img") != string::npos)
	{
		DeleteStr(tabSrc, "<img", ">");
		
	}
	//ȥ��&#160;
	DeleteSubStr(tabSrc,"&#160;");

	onfile << tabSrc;
	onfile.close();
	infile.close();

}

//ɾ��src��btag��etag֮������ݣ�����btag��etag
void DeleteStr(string& src, const string& btag, const string& etag)
{
	size_t bpos = src.find(btag);
	size_t epos = src.find(etag,bpos);
	while (bpos != string::npos)
	{
		src = src.substr(0, bpos) + src.substr(epos+etag.length(),src.length()-epos-etag.length());
		bpos = src.find(btag);
		epos = src.find(etag,bpos);
	}
}

//converts int to string
string itos(int arg)
{
	ostringstream buffer;
	//send the int to the ostringstream
	buffer << arg;

	//capture the string
	return buffer.str();
}

//��ȡsina��ҳ�й��ڹ��Ҷ����ӵ�id��
void GetCountryTeamId()
{
	int attempt = 3; //��ೢ������3��
	string pageSrc;
	string itosNum;
	string initLink = "http://match.sports.sina.com.cn/football/team.php?id=";
	string link;
	int initPos = 922;
	ofstream ofile1;
	
	ofile1.open(cLink);

	do{
		itosNum = itos(initPos);
		link = initLink;
		link.append(itosNum);

		//������ҳ
		pageSrc = DownLoadPage(link);
		while(pageSrc.compare("error")!=string::npos
			&& attempt>0)
		{
			cout << link << " ����ʧ��" << endl;
			Sleep(6000);
			attempt--;
			pageSrc.clear();
			pageSrc = DownLoadPage(link);
		}
		
		//��ȡ��ҳ�еĹ������������ļ���
		string cNameLine; //����������������
		if(pageSrc.find("����")!=string::npos)
		{
			cNameLine = GetTag(pageSrc, "����:", "<br/>",1);

		}
		ofile1 << (cNameLine + " " + itosNum + "\n" );
		
		//��������
		initPos ++;
		itosNum.clear();
		link.clear();
	}while(1);
}

//��ʼ�������������Ƶ��ĵ�
bool InitCnameFile(const string& filename)
{
	ifstream FileToSet;
	string strLine;	//���һ�е�����

	FileToSet.open(nf_country_zh_en_name); //�������Ҷ���Ӣ�������ĵ�
	
	//���ı��е�����ؼ��ʴ������ݼ�CTKey��
	while(getline(FileToSet,strLine))
	{
		//��һ�е�ǰ�������ݷ������ݼ���
		CTKey.insert(strLine);
	}
	FileToSet.close();

	//��nf_country_zh_en_name�ļ��е����ݼ��뵽�ִʹ���ICTCLAS��
	if(!ICTCLAS_Init("E:\\ZJM\\vsProject\\download\\download"))
	{
		//�ڳ�ʼĿ¼�£�Ӧ�ð���Data���ݰ�,configure.xml�Լ�user.lic�ļ�
		//������ɳ�ʼ��
		cout << "�ִʹ��߳�ʼ��ʧ��" << endl;
		return false;
	}
	ICTCLAS_ImportUserDictFile(nf_country_zh_en_name.c_str(), CODE_TYPE_UNKNOWN);
	ICTCLAS_FileProcess(nf_country_zh_en_name.c_str(), "Test_result.txt",CODE_TYPE_GB, 1);
	ICTCLAS_SaveTheUsrDic();//������ӵĴʻ㣬�´�ʹ����Ȼ��Ч
	ICTCLAS_Exit();//�ͷ���Դ�˳�
	return true;
}

