#pragma once

#include "SServerStd.h"

class DataLeader : public SSingleton <DataLeader>
{
private:
	friend class SSingleton<DataLeader>;

public:
	//--------------------------------------------------------------
	int		SessionCount;
	//--------------------------------------------------------------
	int		RoomCount;
	//--------------------------------------------------------------
	char	LoginSrvIP[15];
	int		LoginSrvPortNum;
	//--------------------------------------------------------------
	char	LobbySrvIP[15];
	int		LobbySrvPortNum;
	//--------------------------------------------------------------
	char	GameSrvIP[15];
	int		GameSrvProtNum;
	//--------------------------------------------------------------
	int		GamePlaySec;

public:
	DataLeader(void);
	~DataLeader(void);

public:
	BOOL DataSetting();


	//--------------------------------------------------------------

};

#define GetDocument DataLeader::GetInstance()


