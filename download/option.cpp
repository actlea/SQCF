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
//<>value</>提取的就是中间的value的值
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

//提取标签元素中某个属性的值，仍然分两种类型:<a..></a>或者是<img.../>的类型
string::size_type NextAttribute(const string&source,const string& attribute_name,string &value,string::size_type pos,bool non_end_mark)
{

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
	string tabStr = ExtractDataToStr(pageSrc); //提取的未经处理的表格数据
	string lineStr;
	string::size_type pos = 0;
	string::size_type one,two,three,four,five;
	string tempSrc;

	_sPlayer oPlayer;
	//vector<_sPlayer>::iterator player_iter;
	string ename;
	string birthday;
	string position;
	string club;
	//int col = 5;	//我们只需要前5列的数据，其中有一列是国旗，也要去掉

	//处理表格数据，因为表格式按行划分的，因此我们可以按行读取
	while((pos=ReadLine(tabStr,lineStr,pos))<tabStr.size())
	{
		//提取数据
		one=locate(lineStr,",",1);
		two=locate(lineStr,",",2);
		three=locate(lineStr,",",3);
		four=locate(lineStr,",",4);
		five=locate(lineStr,",",5);
		
		ename=lineStr.substr(0,one);
		ename=extract(ename);
		//名字比较长
		if(ename.find("/")!=string::npos)
		{
			ename.replace(ename.find("/"),1,',');
		}

		birthday=lineStr.substr(one,two-one);
		birthday=extract(birthday);

		position=lineStr.substr(two,three-two);
		position=extract(position);

		club=lineStr.substr(four,five-four);
		club=extract(club);
		//存入数据
		oPlayer._eName=ename;
		oPlayer._age=birthday;
		oPlayer._position=position;
		oPlayer._clubName=club;

		(s_nteam._players).push_back(oPlayer);
		//用于测试
		tempSrc += ename+ birthday+ position +club+"\n";
	}
	string file1 = "./Country/file1.txt";
	ofstream tablefile;
	tablefile.open(file1);
	tablefile<<tempSrc;
	tablefile.close();
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

//按行读取字符串,返回行结束符的位置
string::size_type ReadLine(const string& tableStr,string&lineStr,string::size_type pos)
{
	string::size_type pbegin = tableStr.find('\n',pos+1);
	lineStr.clear();
	if(pbegin==string::npos)
	{
		return tableStr.size();
	}
	lineStr = tableStr.substr(pos,pbegin);
	return pbegin;
}

string::size_type locate(string &line,const string& tag,int index)
{
	string::size_type pos = 0;
	string::size_type tagPos=pos;
	int i=1;
	while(i<=index)
	{
		tagPos=line.find(tag,pos);
		if(tagPos!=string::npos)
			i++;
		pos = tagPos+1;
	}
	if(i<=index)
		return line.size();
	else
		return tagPos;
}

string::size_type locate(string &line,const string&tag)
{
	return locate(line,tag,1);
}

//考虑的只有两种情形
//<a><></></a>和...></>
//不需要配套地找<>和</>，而只需要知道最靠近数据的></>就可以了
string extract(string&line)
{
	string::size_type pre=0,start=0;
	eraseTag(line,',');
	if(line.find("<")==string::npos)
		return line;
	while((start=line.find("<",start))!=string::npos)
	{
		if(line[start+1]=='/')
		{
			start++;
			continue;
		}
		else
		{
			pre=start;
			start++;
		}
	}
	//此时pre指向最靠近数据的一个标签
	//if(line[pre]!='<')//说明标签不是完全匹配的，有缺损，比如..></>
		//start=pre;
	//else
		start=line.find(">",pre);

	string::size_type end=line.find("</",start+1);
	if(end==string::npos)
		end=line.size();
	return line.substr(start+1,end-start-1);
}

//去除单个的tag标签
void eraseTag(string &source,char tag)
{
	string::iterator it_str;
	for(it_str=source.begin();it_str!=source.end();it_str++)
	{
		if((*it_str)==tag)
			source.erase(it_str);
	}
}

//****************************提取表格数据******************

//从下载好的网页中提取表格存入string中
std::string ExtractDataToStr(string& str)
{
	std::string originalPageString = str;
	std::string tabStr;
	std::string tempStr;
	std::string sResult;
	
	while (FindTableInHtmlString(originalPageString, tempStr) == 0)
	{	

		tabStr += tempStr;
		//从表格中提取数据
		sResult+=ExtractTableDataEx2(tabStr);
				
	}/*end while*/
	return sResult;
}

//该函数去除了原始table中所有的换行符，然后最后按行提取的时候又加上了换行符，
//这样我们就可以按行读取字符串了
std::string ExtractTableDataEx2(string& table)
{
	string strResult;
	//去除表格中的空格
	DeleteSpaces(table);
	eraseTag(table,'\n');//去除原始table中所有的换行符

	//去掉表头<thead
	if(table.find("tbody")!=string::npos)
		table = table.substr(table.find("<tbody"),table.find("</tbody")-table.find("<tbody"));
	//统计表格行数
	int rowNum = SubStrCount(table, "<tr");


	//以表格各行的最大列数作为表格的列数
	int colNum = 0;
	size_t rpos = 0;
	string rowStr;
	for (int i = 0; i < rowNum; i++)
	{
		rpos = FindStr2Str(table, rpos, "<tr", "</tr>", rowStr);
		int n = SubStrCount(rowStr, "<td");
		if (n > colNum)
			colNum = n;
	}

	
	//初始化包含所有表格数据的二维vector
	vector< vector<string> > tabVec(rowNum);
	vector< vector<string> >::iterator rIter = tabVec.begin();
	for (; rIter != tabVec.end(); ++rIter)
	{
		(*rIter).resize(colNum);
	}

	//按行遍历表格
	rpos = 0;
	for (int i = 0; i < rowNum; i++)
	{
		rpos = FindStr2Str(table, rpos, "<tr", "</tr>", rowStr);
		//计算这一行的列数
		int crrntColNum = SubStrCount(rowStr, "<td") + SubStrCount(rowStr, "<th");
		
		//对每一个td进行处理
		string tdStr, dataStr, tmpStr;
		int cpos = 0;
		for (int j = 0; j < crrntColNum; j++)
		{
			if (rowStr.find("<th") != string::npos)
				cpos = FindStr2Str(rowStr, cpos, "<th", "</th>", tdStr);
			else
				cpos = FindStr2Str(rowStr, cpos, "<td", "</td>", tdStr);
			
			//确定该单元格横跨的行数
			int spanNum;
			if (tdStr.find("rowspan") != string::npos)
			{
				string spanNumStr;
				FindStr2StrEx(tdStr, 0, "rowspan=\"", "\"", spanNumStr);
				spanNum = atoi(spanNumStr.c_str());
			}
			else
				spanNum = 1;
			//为了防止网页本身rowspan错误导致tabVec数组越界，将spanNum修改为正确值
			//该情况出现在http://sports.sina.com.cn/z/wvbach11/schedule/index.shtml 第9比赛日9.23(周五)这个字段上面
			if (i + spanNum - 1 > rowNum - 1)
				spanNum = rowNum - i;
			//提取单元格的数据
			ExtractDataFromTd(tdStr, dataStr);

			
			//在第i行的vector中找到第一个为空的位置
			int emptyPos = 0;
			while (tabVec[i][emptyPos] != "")
				emptyPos++;
			//对spanNum行写入单元格数据dataStr
			for (int k = i; k - i < spanNum; k++)
			{
				
				tabVec[k][emptyPos] = dataStr;
			}
		}
	}
	//将二维vector写入string
	rIter = tabVec.begin();
	for (; rIter != tabVec.end(); ++rIter)
	{
		vector<string>::iterator cIter = (*rIter).begin();
		for (; cIter <= (*rIter).end() - 1; ++cIter)
		{
			//不是最后一项，且下一项不为空
			if (cIter != (*rIter).end() - 1 && *cIter != "" && *(cIter+1) != "")
				strResult += (*cIter) + ',';
			else
			{
				strResult += (*cIter);
				break;
			}
		}
		strResult += '\n';
	}
	return strResult;
}

//从本地文件中读取数据存入pageSrc，如果成功返回true
bool ReadFromFile(string filePath,string& pageSrc)
{
	string tmpStr;
	ifstream in;

	in.open(filePath);
	if(!in)
		return false;

	
	while(getline(in,tmpStr))
	{
		pageSrc+=tmpStr;
		tmpStr.clear();
	}
	in.close();
	return true;
}

bool WriteToFile(string filePath,string& pageSrc)
{
	string tmpStr;
	ofstream out;

	out.open(filePath);
	if(!out)
		return false;
	out<<pageSrc;
		return true;
}