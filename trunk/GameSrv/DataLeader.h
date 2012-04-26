#pragma once

#include "SServerStd.h"

class DataLeader : public SSingleton <DataLeader>
{
private:
	friend class SSingleton<DataLeader>;

public:
	//--------------------------------------------------------------
	int		RoomCount;
	//--------------------------------------------------------------
	char	GameSrvIP[15];
	int		GameSrvProtNum;
	//--------------------------------------------------------------
	char	LobbySrvIP[15];
	int		LobbySrvPortNum;
	//--------------------------------------------------------------
	int		GamePlaySec;

public:
	DataLeader(void);
	~DataLeader(void);

public:
	BOOL DataSetting();


	//--------------------------------------------------------------
	
};

#define GetData DataLeader::GetInstance()


