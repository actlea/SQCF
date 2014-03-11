#include "option.h"
#include "Table.h"

using namespace std;

std::set<string> CTKey;
/**
*source为源字符串 bTag为起始标签 eTag为结束标签 
*flag为0时，返回的字符包括标签部分
*flag为1时，返回的字符不包括标签部分，默认pos=0
*@返回source中bTag和eTag之间的字符
*/
string GetTag(const string& source, const string& bTag, const string& eTag,int flag)
{
	string sContent;
	string::size_type adr1=0,adr2=0;

	adr1 = source.find(bTag);
	adr2 = source.find(eTag,adr1+bTag.length());
	if(adr1==string::npos
		|| adr2 ==string::npos)
	{
		//cout << "不含有标签" << endl;
		return sContent;
	}
	if(flag==0)
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

/******************************************
	*Description:判断是否是我们需要的链接,
	*			一般LinkText的形式为<a ...>...</a>
	*Call:GetTag
	*/
bool JudgeFootballInfo(const string& LinkText)
{
	//查看href中的信息
	string hrefText = GetTag(LinkText,"href=\"","\"");
	if(hrefText.find("country")!=string::npos
		||hrefText.find("continent")!=string::npos
		||hrefText.find("player")!=string::npos)
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

//*************************************************************
//
//获取sina网页中关于国家队链接的id号
//经过测试，无法发现网页id号的规律，因此无法作为资源网页使用
void GetCountryTeamId()
{
	int attempt = 3; //最多尝试下载3次
	string pageSrc;
	string itosNum;
	string initLink = "match.sports.sina.com.cn/football/team.php?id=";
	string link;
	int initPos = 922;
	int count = 0;
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
		cNameLine = GetTag(pageSrc, "<title>", "</title>",1);
	
		ofile1 << (cNameLine + " " + itosNum + "\n" );
		
		//后续处理
		initPos ++;
		itosNum.clear();
		link.clear();
		count ++;
	}while(count<300);
}


//初始化包含国家名称的文档
bool InitCnameFile()
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

//提取国家队教练
void ExtractCountryCoach()
{
	ofstream outer;
	ifstream inner;
	string tempStr;
	string value;
	string str;
	string::size_type pos;
	ExtractTableDataFromUrl(SDNCOACHURL, COACHN);
	
	outer.open(NCOACHN,ios::app);
	inner.open(COACHN);
	if(!outer||!inner)
	{
		cout << "文件打开失败！" << endl;
	}
	outer << "排名,国家,英文名,积分,教练\n";
	//按行提取标签中的有用信息
	while(getline(inner,tempStr))
	{
		pos = NextValue(tempStr,0,"font",str);//提取<font..></font>标签的内容
		if(pos==tempStr.size())
		{
			continue;
		}
		value += str+",";
		for(int i=0; i<3; i++)
		{
			str.clear();
			pos = NextValue(tempStr,pos,"a",str);
			value += str+",";
		    //没有经过标签修饰的积分信息
			if(i==1)
			{
				value += tempStr.substr(pos+1,tempStr.find("<a",pos)-pos-1);
			}
		}
		value += "\n";
	 }
	outer << value;
	outer.close();
	inner.close();

}

//提取标签中的内容，返回标签结束的位置
string::size_type NextValue(const  string& source,string::size_type pos,const string& element_name,string& value,bool non_end_mark)
{
	string element="";
    string::size_type pelement_end=NextElement(source,pos,element_name,element);//position right behind the element
	if(element==""){
		value="";
		return source.size();
	}
    string::size_type pos_t=0;
    string::size_type pre_pos=0;
    string::size_type pend1=element.find("/>");
    string::size_type pend2=element.find("</");
    string::size_type pend=pend1;
    if (pend1==string::npos)
        pend=pend2;
    else if (pend2==string::npos)
        pend=pend1;
    else
    pend=(pend1>pend2)?pend1:pend2;
    string first_part=element.substr(0,pend);
    while((pos_t=first_part.find(">",pos_t))!=string::npos)
    {
       if(first_part[pos_t-1]!='/')
               pre_pos=pos_t++;
        else
            break;

    }//after this loop.pre_pos point to the last elememt's '>'
    pre_pos++;
    value=element.substr(pre_pos,element.find("<",pre_pos)-pre_pos);
    return pelement_end;
}

//提取source遇到的第一个element_name的标签
//返回的element如果不为空，就应该是<element_name...>value</element_name>

string::size_type NextElement(const  string&source,string::size_type pos,const string&element_name,string&element,bool non_end_mark)
{
	element="";
    string match_name="<"+element_name;
   // 找到类型<@element_name ...>的标记
    string::size_type pbegin=pos;
	string::size_type pend=pbegin;
	while(pbegin<source.size()){
		pbegin=search(pbegin+source.begin(),source.end(),match_name.begin(),match_name.end(),nocase_compare)-source.begin();
		if(pbegin>source.size()) return source.size();
		pend=pbegin+element_name.size()+1;
		if(pend>source.size()) return source.size();
		if(source[pend]=='/'){
			pend+=1;
			if(pend>source.size()) return source.size();
			if(source[pend]=='>'){
				//找到类型<@element_name/>的标记
				element=source.substr(pbegin,pend-pbegin+1);
				return pend+1;
			}
		}else if(source[pend]=='>'||source[pend]==' '){
			//找到类型<@element_name>的标记的开始部分或<@element_name .....
			break;
		}else{
			pbegin++;
		}
	}//循环结束后pbegin指向@source中第一个<@element_name  .........或<<@element_name>的第一个元素,pend指向@element_name后面的一个字符
	if(pbegin>source.size()) return source.size();
	if(source[pend]!='>'){
		pend=source.find(">",pend);
		if(pend==string::npos)
			return source.size();
	}
	if(non_end_mark){
		element=source.substr(pbegin,pend-pbegin+1);
		return pend+1;
	}
    match_name="</"+element_name+">";
    pend=search(pend+source.begin(),source.end(),match_name.begin(),match_name.end(),nocase_compare)-source.begin();
    if(pend==source.size())
        return source.size();
    else
    {
        pend+=match_name.size();
        element=source.substr(pbegin,pend-pbegin);
    }
    return pend;
}

bool nocase_compare(char c1,char c2)
{
   return toupper(c1)==toupper(c2);
}

//判断网址所代表的网页内型
URL UrlFlag(const string&url)
{
	URL urlF;
	if(url.find("continent")!=string::npos) urlF=CONTINENT;
	else if(url.find("country")!=string::npos) urlF=COUNTRY;
	else if(url.find("player")!=string::npos) urlF=PLAYER;
	else urlF=HOME;
	return urlF;
}

//提取网页中的超链接，并按类型进行归类
std::map<string,URL> GetHyperLinks(const string& source)
{
	std::map<string,URL>linkMap;
	string::size_type srclen = source.length();
	string::size_type i=0;
	URL e;
	string element="";
	string link; //链接文本
	while((i=NextElement(source,i,"a",element))<srclen)
	{
		//先判断是否含有足球类链接，再决定是否提取
		if(JudgeFootballInfo(element))
		{
			link = SelectURL(element);
			e = UrlFlag(link);
			//原链接集合中不包含link,则加入
			if(!linkMap.count(link))
			{
				linkMap.insert(pair<string,URL>(link,e));
			}
		}

	}//end while
	return linkMap;
}

void CountryPage(const string& source, _sNTeam& s_nteam)
{
	string pageSrc = source;
//	string tabStr = ExtractDataToStr(pageSrc);
	string file1 = "./Country/file1.txt";
	ExtractTableDataFromUrl(source, file1);

}



//提取球员信息
void PlayerPage(const string& source,_sPlayer& s_player)
{
	
}

//获取date
void GetDate(_sDate& Date)
{
	time_t timer;
	struct tm *tblock;
	timer = time(NULL);/* get current time; same as: timer = time(&timer)  */
	tblock = localtime(&timer);
	Date._year = tblock->tm_year+1900;
	Date._month = tblock->tm_mon+1;
	Date._day = tblock->tm_mday;
}
