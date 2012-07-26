#pragma once

#include "SServerStd.h"

//======================================
// 서버포트 번호
//======================================
#define DBSRV_PORTNUM		9000
#define LOGINSRV_PORTNUM	8800
#define LOBBYSRV_PORTNUM	8900
#define GAMESRV_PORTNUM		7900
#define LOGSRV_PORTNUM		7800
//======================================

class DataLeader : public SSingleton <DataLeader>
{
private:
	friend class SSingleton<DataLeader>;

public:
	//--------------------------------------------------------------
	int		SessionCount;
	//--------------------------------------------------------------
	BOOL	isAutoSrvSetting;
	//--------------------------------------------------------------
	char	DBSrvIp[15];
	int		DBSrvPortNum;
	//--------------------------------------------------------------
	char	LoginSrvIP[15];
	int		LoginSrvPortNum;
	//--------------------------------------------------------------
	char	LobbySrvIP[15];
	int		LobbySrvPortNum;
	//--------------------------------------------------------------
	char	GameSrvIP[15];
	int		GameSrvPortNum;
	//--------------------------------------------------------------
	char	LogSrvIP[15];
	int		LogSrvPortNum;
	//--------------------------------------------------------------

public:
	DataLeader(void);
	~DataLeader(void);

public:
	BOOL DataSetting();
	//--------------------------------------------------------------
private:
	//현재 컴퓨터의 ip주소를 받아 자동으로 셋팅한다
	void AutoServerSetting();
};

#define GetDocument DataLeader::GetInstance()


