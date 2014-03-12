#ifndef TABLE_H
#define TABLE_H
#include <string>
#include <vector>
#include "download.h"

//һ��������ÿ����������Ϣ
typedef struct
{
	std::string date; //����
	std::string time; //����ʱ��
	std::string timeLocal; //����ʱ��
	std::string round; //�ִ�
	std::string order; //���򡢱�������
	std::string group; //��𡢷���
	std::string tableNo; //̨������
	std::string player1; //��Ա1�����1������
	std::string player2; //��Ա2�����2���Ͷ�
	std::string score; //�ȷ�
	std::string result; //����
	std::string detail; //��ϸ��Ϣ
	std::string stage; //�׶�
	std::string vs; //���󣨰�����Ա1����Ա2��������
	std::string location; //�ص㡢���ݡ�����
	std::string bestPlayer; //ÿ�������Ա
} PerMatchInfo;

//һ�����µ���Ϣ
typedef struct
{
	std::string season; //������Ϣ�����ڸ�������
	std::string startDate; //���¿�ʼ����
	std::string endDate; //���½�������
	std::string eventName; //��������
	std::string eventLevel; //���˹ŵ�ˣ������������������ʸ�����
	std::string eventStars; //��Ҫ�ȣ��ʺϻ�ʽײ��
	std::string eventType; //���̨��˹ŵ�ˡ���ʽײ����ʽ̨���
	std::string venue; //���¾ٰ��
	std::string detail; //��������
	std::vector<PerMatchInfo> matchInfo; //����ÿ����������Ϣ
} EventInfo;


//ȫ�ֱ���
//extern std::vector<std::string> eventDict, detailDict;
extern std::vector<std::string> dict;

int InitDict();
int ExtractTableFromHtml(const char *inPath, const char *outPath);
int FindTableInHtmlString(std::string &htmlStr, std::string &tabStr);
//std::string FindTableInHtmlString(std::string &htmlStr);
int TableInfoType(const std::string &table);
int ExtractTableData(const char *inPath, const char *outPath);
//int ExtractTableRow(const std::string &rowStr, std::string &csStr);
void ExtractDataFromTd(std::string &tdStr, std::string &dataStr);
//��ȡ���������е�Ԫ��Ĳ�������
int ExtractTableDataEx(const char *inPath, const char *outPath);

void DownloadHtmlFile(const std::string &url, const std::string &filePath);
//�Ӹ�����Url����ȡ����Ҫ��ı������
void ExtractTableDataFromUrl(const std::string &url, const std::string &tdPath);
//�Ӹ�����html�ļ�����ȡ����Ҫ��ı������
void ExtractTableDataFromFile(const std::string &htmlPath, const std::string &tdPath);


#endif // !TABLE_H


