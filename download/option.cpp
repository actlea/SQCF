#include "option.h"
#include "Table.h"

using namespace std;

std::set<string> CTKey;
/**
*sourceΪԴ�ַ��� bTagΪ��ʼ��ǩ eTagΪ������ǩ 
*flagΪ0ʱ�����ص��ַ�������ǩ����
*flagΪ1ʱ�����ص��ַ���������ǩ���֣�Ĭ��pos=0
*@����source��bTag��eTag֮����ַ�
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
		//cout << "�����б�ǩ" << endl;
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
	*Description:�ж��Ƿ���������Ҫ������,
	*			һ��LinkText����ʽΪ<a ...>...</a>
	*Call:GetTag
	*/
bool JudgeFootballInfo(const string& LinkText)
{
	//�鿴href�е���Ϣ
	string hrefText = GetTag(LinkText,"href=\"","\"");
	if(hrefText.find("country")!=string::npos
		||hrefText.find("continent")!=string::npos
		||hrefText.find("player")!=string::npos)
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

//*************************************************************
//
//��ȡsina��ҳ�й��ڹ��Ҷ����ӵ�id��
//�������ԣ��޷�������ҳid�ŵĹ��ɣ�����޷���Ϊ��Դ��ҳʹ��
void GetCountryTeamId()
{
	int attempt = 3; //��ೢ������3��
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
		cNameLine = GetTag(pageSrc, "<title>", "</title>",1);
	
		ofile1 << (cNameLine + " " + itosNum + "\n" );
		
		//��������
		initPos ++;
		itosNum.clear();
		link.clear();
		count ++;
	}while(count<300);
}


//��ʼ�������������Ƶ��ĵ�
bool InitCnameFile()
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

//��ȡ���Ҷӽ���
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
		cout << "�ļ���ʧ�ܣ�" << endl;
	}
	outer << "����,����,Ӣ����,����,����\n";
	//������ȡ��ǩ�е�������Ϣ
	while(getline(inner,tempStr))
	{
		pos = NextValue(tempStr,0,"font",str);//��ȡ<font..></font>��ǩ������
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
		    //û�о�����ǩ���εĻ�����Ϣ
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

//��ȡ��ǩ�е����ݣ����ر�ǩ������λ��
//<>value</>��ȡ�ľ����м��value��ֵ
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

//��ȡsource�����ĵ�һ��element_name�ı�ǩ
//���ص�element�����Ϊ�գ���Ӧ����<element_name...>value</element_name>

string::size_type NextElement(const  string&source,string::size_type pos,const string&element_name,string&element,bool non_end_mark)
{
	element="";
    string match_name="<"+element_name;
   // �ҵ�����<@element_name ...>�ı��
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
				//�ҵ�����<@element_name/>�ı��
				element=source.substr(pbegin,pend-pbegin+1);
				return pend+1;
			}
		}else if(source[pend]=='>'||source[pend]==' '){
			//�ҵ�����<@element_name>�ı�ǵĿ�ʼ���ֻ�<@element_name .....
			break;
		}else{
			pbegin++;
		}
	}//ѭ��������pbeginָ��@source�е�һ��<@element_name  .........��<<@element_name>�ĵ�һ��Ԫ��,pendָ��@element_name�����һ���ַ�
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

//��ȡ��ǩԪ����ĳ�����Ե�ֵ����Ȼ����������:<a..></a>������<img.../>������
string::size_type NextAttribute(const string&source,const string& attribute_name,string &value,string::size_type pos,bool non_end_mark)
{

}

bool nocase_compare(char c1,char c2)
{
   return toupper(c1)==toupper(c2);
}

//�ж���ַ���������ҳ����
URL UrlFlag(const string&url)
{
	URL urlF;
	if(url.find("continent")!=string::npos) urlF=CONTINENT;
	else if(url.find("country")!=string::npos) urlF=COUNTRY;
	else if(url.find("player")!=string::npos) urlF=PLAYER;
	else urlF=HOME;
	return urlF;
}

//��ȡ��ҳ�еĳ����ӣ��������ͽ��й���
std::map<string,URL> GetHyperLinks(const string& source)
{
	std::map<string,URL>linkMap;
	string::size_type srclen = source.length();
	string::size_type i=0;
	URL e;
	string element="";
	string link; //�����ı�
	while((i=NextElement(source,i,"a",element))<srclen)
	{
		//���ж��Ƿ������������ӣ��پ����Ƿ���ȡ
		if(JudgeFootballInfo(element))
		{
			link = SelectURL(element);
			e = UrlFlag(link);
			//ԭ���Ӽ����в�����link,�����
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
	string tabStr = ExtractDataToStr(pageSrc); //��ȡ��δ������ı������
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
	//int col = 5;	//����ֻ��Ҫǰ5�е����ݣ�������һ���ǹ��죬ҲҪȥ��

	//���������ݣ���Ϊ���ʽ���л��ֵģ�������ǿ��԰��ж�ȡ
	while((pos=ReadLine(tabStr,lineStr,pos))<tabStr.size())
	{
		//��ȡ����
		one=locate(lineStr,",",1);
		two=locate(lineStr,",",2);
		three=locate(lineStr,",",3);
		four=locate(lineStr,",",4);
		five=locate(lineStr,",",5);
		
		ename=lineStr.substr(0,one);
		ename=extract(ename);
		//���ֱȽϳ�
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
		//��������
		oPlayer._eName=ename;
		oPlayer._age=birthday;
		oPlayer._position=position;
		oPlayer._clubName=club;

		(s_nteam._players).push_back(oPlayer);
		//���ڲ���
		tempSrc += ename+ birthday+ position +club+"\n";
	}
	string file1 = "./Country/file1.txt";
	ofstream tablefile;
	tablefile.open(file1);
	tablefile<<tempSrc;
	tablefile.close();
}



//��ȡ��Ա��Ϣ
void PlayerPage(const string& source,_sPlayer& s_player)
{
	
}

//��ȡdate
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

//���ж�ȡ�ַ���,�����н�������λ��
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

//���ǵ�ֻ����������
//<a><></></a>��...></>
//����Ҫ���׵���<>��</>����ֻ��Ҫ֪��������ݵ�></>�Ϳ�����
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
	//��ʱpreָ��������ݵ�һ����ǩ
	//if(line[pre]!='<')//˵����ǩ������ȫƥ��ģ���ȱ�𣬱���..></>
		//start=pre;
	//else
		start=line.find(">",pre);

	string::size_type end=line.find("</",start+1);
	if(end==string::npos)
		end=line.size();
	return line.substr(start+1,end-start-1);
}

//ȥ��������tag��ǩ
void eraseTag(string &source,char tag)
{
	string::iterator it_str;
	for(it_str=source.begin();it_str!=source.end();it_str++)
	{
		if((*it_str)==tag)
			source.erase(it_str);
	}
}

//****************************��ȡ�������******************

//�����غõ���ҳ����ȡ������string��
std::string ExtractDataToStr(string& str)
{
	std::string originalPageString = str;
	std::string tabStr;
	std::string tempStr;
	std::string sResult;
	
	while (FindTableInHtmlString(originalPageString, tempStr) == 0)
	{	

		tabStr += tempStr;
		//�ӱ������ȡ����
		sResult+=ExtractTableDataEx2(tabStr);
				
	}/*end while*/
	return sResult;
}

//�ú���ȥ����ԭʼtable�����еĻ��з���Ȼ���������ȡ��ʱ���ּ����˻��з���
//�������ǾͿ��԰��ж�ȡ�ַ�����
std::string ExtractTableDataEx2(string& table)
{
	string strResult;
	//ȥ������еĿո�
	DeleteSpaces(table);
	eraseTag(table,'\n');//ȥ��ԭʼtable�����еĻ��з�

	//ȥ����ͷ<thead
	if(table.find("tbody")!=string::npos)
		table = table.substr(table.find("<tbody"),table.find("</tbody")-table.find("<tbody"));
	//ͳ�Ʊ������
	int rowNum = SubStrCount(table, "<tr");


	//�Ա����е����������Ϊ��������
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

	
	//��ʼ���������б�����ݵĶ�άvector
	vector< vector<string> > tabVec(rowNum);
	vector< vector<string> >::iterator rIter = tabVec.begin();
	for (; rIter != tabVec.end(); ++rIter)
	{
		(*rIter).resize(colNum);
	}

	//���б������
	rpos = 0;
	for (int i = 0; i < rowNum; i++)
	{
		rpos = FindStr2Str(table, rpos, "<tr", "</tr>", rowStr);
		//������һ�е�����
		int crrntColNum = SubStrCount(rowStr, "<td") + SubStrCount(rowStr, "<th");
		
		//��ÿһ��td���д���
		string tdStr, dataStr, tmpStr;
		int cpos = 0;
		for (int j = 0; j < crrntColNum; j++)
		{
			if (rowStr.find("<th") != string::npos)
				cpos = FindStr2Str(rowStr, cpos, "<th", "</th>", tdStr);
			else
				cpos = FindStr2Str(rowStr, cpos, "<td", "</td>", tdStr);
			
			//ȷ���õ�Ԫ���������
			int spanNum;
			if (tdStr.find("rowspan") != string::npos)
			{
				string spanNumStr;
				FindStr2StrEx(tdStr, 0, "rowspan=\"", "\"", spanNumStr);
				spanNum = atoi(spanNumStr.c_str());
			}
			else
				spanNum = 1;
			//Ϊ�˷�ֹ��ҳ����rowspan������tabVec����Խ�磬��spanNum�޸�Ϊ��ȷֵ
			//�����������http://sports.sina.com.cn/z/wvbach11/schedule/index.shtml ��9������9.23(����)����ֶ�����
			if (i + spanNum - 1 > rowNum - 1)
				spanNum = rowNum - i;
			//��ȡ��Ԫ�������
			ExtractDataFromTd(tdStr, dataStr);

			
			//�ڵ�i�е�vector���ҵ���һ��Ϊ�յ�λ��
			int emptyPos = 0;
			while (tabVec[i][emptyPos] != "")
				emptyPos++;
			//��spanNum��д�뵥Ԫ������dataStr
			for (int k = i; k - i < spanNum; k++)
			{
				
				tabVec[k][emptyPos] = dataStr;
			}
		}
	}
	//����άvectorд��string
	rIter = tabVec.begin();
	for (; rIter != tabVec.end(); ++rIter)
	{
		vector<string>::iterator cIter = (*rIter).begin();
		for (; cIter <= (*rIter).end() - 1; ++cIter)
		{
			//�������һ�����һ�Ϊ��
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

//�ӱ����ļ��ж�ȡ���ݴ���pageSrc������ɹ�����true
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