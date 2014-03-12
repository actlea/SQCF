#ifndef TABLE_H
#define TABLE_H
#include <string>
#include <vector>
#include "download.h"

//一项赛事中每场比赛的信息
typedef struct
{
	std::string date; //日期
	std::string time; //北京时间
	std::string timeLocal; //当地时间
	std::string round; //轮次
	std::string order; //场序、比赛场次
	std::string group; //组别、分组
	std::string tableNo; //台球桌号
	std::string player1; //球员1、球队1、主队
	std::string player2; //球员2、球队2、客队
	std::string score; //比分
	std::string result; //赛果
	std::string detail; //详细信息
	std::string stage; //阶段
	std::string vs; //对阵（包括球员1、球员2、赛果）
	std::string location; //地点、场馆、场地
	std::string bestPlayer; //每场最佳球员
} PerMatchInfo;

//一项赛事的信息
typedef struct
{
	std::string season; //赛季信息，用于更新赛程
	std::string startDate; //赛事开始日期
	std::string endDate; //赛事结束日期
	std::string eventName; //赛事名称
	std::string eventLevel; //如对斯诺克：排名赛、邀请赛、资格赛等
	std::string eventStars; //重要度，适合花式撞球
	std::string eventType; //如对台球：斯诺克、花式撞球、中式台球等
	std::string venue; //赛事举办地
	std::string detail; //赛事详情
	std::vector<PerMatchInfo> matchInfo; //包含每场比赛的信息
} EventInfo;


//全局变量
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
//提取包含横跨多行单元格的不规则表格
int ExtractTableDataEx(const char *inPath, const char *outPath);

void DownloadHtmlFile(const std::string &url, const std::string &filePath);
//从给定的Url中提取满足要求的表格数据
void ExtractTableDataFromUrl(const std::string &url, const std::string &tdPath);
//从给定的html文件中提取满足要求的表格数据
void ExtractTableDataFromFile(const std::string &htmlPath, const std::string &tdPath);


#endif // !TABLE_H


