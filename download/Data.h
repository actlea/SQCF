#pragma once

#include "stdafx.h"
#include <string>
using std::string;

typedef struct _Player
{
	string _cName;//������
	string _eName;//Ӣ����
	string _imageLink;//��Ƭ
	string _age;
	string _height;
	string _Nation;//����
	string _clubName;//���ֲ�
	string _birthPlace;
	string _position;//���ϵ�λ��
}_sPlayer;


typedef struct _NTeam
{
	string _cCName;//����������
	string _eCNAME;//����Ӣ����
	string _Coach;//����
	string _FlagLink;//����

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