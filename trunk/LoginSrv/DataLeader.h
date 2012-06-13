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
	//--------------------------------------------------------------
	char	LogSrvIP[15];
	int		LogSrvPortNum;
	//--------------------------------------------------------------
	char	DBSrvIp[15];
	int		DBSrvPortNum;
	//--------------------------------------------------------------

public:
	DataLeader(void);
	~DataLeader(void);

public:
	BOOL DataSetting();

	//--------------------------------------------------------------

};

#define GetDocument DataLeader::GetInstance()


