#include "option.h"

using namespace std;

std::set<string> CTKey;
/**
*source为源字符串 bTag为起始标签 eTag为结束标签 
*pos为0时，返回的字符包括标签部分
*pos为1时，返回的字符不包括标签部分，默认pos=0
*@返回source中bTag和eTag之间的字符
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
		cout << "不含有标签" << endl;
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

//判断是否是我们需要的链接
//一般LinkText的形式为<a ...>...</a>
bool JudgeFootballInfo(const string& LinkText)
{
	//查看href中的信息
	string hrefText = GetTag(LinkText,"href=\"","\"");
	if(hrefText.find("country")!=string::npos
		||hrefText.find("continent")!=string::npos)
	{
		return true;
	}
	return false;
}

//从带有链接的锚文本中提取出超链接
string SelectURL(const string& AnchorText)
{
	string LinkURL;
	LinkURL = GetTag(AnchorText,"href=\"","\"",1); //不包括标签部分	
	return LinkURL;
}

//从wiki中提取需要的表格数据
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

	string linestr; //按行读取
	string tabSrc;	//表格数据
	string tmpStr;
	while(getline(infile,linestr))
		tabSrc += linestr + "\n";

	//去除链接<a
	while (tabSrc.find("<a") != string::npos)
	{
		DeleteTag(tabSrc, "a", tmpStr);
		tabSrc = tmpStr;
	}
	//去除img
	if(tabSrc.find("<img") != string::npos)
	{
		DeleteStr(tabSrc, "<img", ">");
		
	}
	//去除&#160;
	DeleteSubStr(tabSrc,"&#160;");

	onfile << tabSrc;
	onfile.close();
	infile.close();

}

//删除src中btag和etag之间的内容，包括btag和etag
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

//获取sina网页中关于国家队链接的id号
void GetCountryTeamId()
{
	int attempt = 3; //最多尝试下载3次
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

		//下载网页
		pageSrc = DownLoadPage(link);
		while(pageSrc.compare("error")!=string::npos
			&& attempt>0)
		{
			cout << link << " 下载失败" << endl;
			Sleep(6000);
			attempt--;
			pageSrc.clear();
			pageSrc = DownLoadPage(link);
		}
		
		//提取网页中的国家姓名存入文件中
		string cNameLine; //包含国家姓名的行
		if(pageSrc.find("国籍")!=string::npos)
		{
			cNameLine = GetTag(pageSrc, "国籍:", "<br/>",1);

		}
		ofile1 << (cNameLine + " " + itosNum + "\n" );
		
		//后续处理
		initPos ++;
		itosNum.clear();
		link.clear();
	}while(1);
}

//初始化包含国家名称的文档
bool InitCnameFile(const string& filename)
{
	ifstream FileToSet;
	string strLine;	//存放一行的数据

	FileToSet.open(nf_country_zh_en_name); //包含国家队中英文名的文档
	
	//把文本中的足球关键词存入数据集CTKey中
	while(getline(FileToSet,strLine))
	{
		//将一行的前两个数据放入数据集中
		CTKey.insert(strLine);
	}
	FileToSet.close();

	//把nf_country_zh_en_name文件中的数据加入到分词工具ICTCLAS中
	if(!ICTCLAS_Init("E:\\ZJM\\vsProject\\download\\download"))
	{
		//在初始目录下，应该包含Data数据包,configure.xml以及user.lic文件
		//才能完成初始化
		cout << "分词工具初始化失败" << endl;
		return false;
	}
	ICTCLAS_ImportUserDictFile(nf_country_zh_en_name.c_str(), CODE_TYPE_UNKNOWN);
	ICTCLAS_FileProcess(nf_country_zh_en_name.c_str(), "Test_result.txt",CODE_TYPE_GB, 1);
	ICTCLAS_SaveTheUsrDic();//保存添加的词汇，下次使用依然有效
	ICTCLAS_Exit();//释放资源退出
	return true;
}

