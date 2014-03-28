#pragma once

#include "stdafx.h"
#include <string>
using std::string;

typedef struct _Player _sPlayer;
typedef struct _NTeam _sNTeam;
typedef struct _Date _sDate;
typedef struct _Coach _sCoach;

typedef struct _Player
{
	string _cName;//������
	string _eName;//Ӣ����
	string _imageLink;//��Ƭ
	string _birthdate;
	string _height;
	string _Nation;//����
	string _clubName;//���ֲ�
	string _birthPlace;
	string _position;//���ϵ�λ��
	string _otherPosition;//���ϵ�����λ��
}_sPlayer;


typedef struct _NTeam
{
	string _cCName;//����������
	string _eCNAME;//����Ӣ����
	
	string _FlagLink;//����
	struct _Coach *pCoach;//����
	string _info;//���
	string _credit;//����
	string _teamImage;//�����Ƭ
	std::vector<_sPlayer> _players;
}_sNTeam;

typedef struct _Date
{
	int _year;
	int _day;
	int _month;
}_sDate;

typedef struct _Coach
{
	string _cname;
	string _ename;
	string _imageurl;
	string _birthdate;
	string _birthPlace;
	string _height;
}_sCoach;