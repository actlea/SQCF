#pragma once

#include "stdafx.h"
#include <string>
using std::string;

typedef struct _Player
{
	string _cName;//中文名
	string _eName;//英文名
	string _imageLink;//照片
	string _age;
	string _height;
	string _Nation;//国籍
	string _clubName;//俱乐部
	string _birthPlace;
	string _position;//场上的位置
}_sPlayer;


typedef struct _NTeam
{
	string _cCName;//国家中文名
	string _eCNAME;//国家英文名
	string _Coach;//教练
	string _FlagLink;//国旗

	string _info;//简介
	string _credit;//荣誉
	string _teamImage;//球队照片
	std::vector<_sPlayer> _players;
}_sNTeam;

typedef struct _Date
{
	int _year;
	int _day;
	int _month;
}_sDate;