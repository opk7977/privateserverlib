#include "DataLeader.h"

DataLeader::DataLeader(void)
: SessionCount(0)
, RoomCount(0)
, LoginSrvPortNum(0)
, LobbySrvPortNum(0)
, GameSrvPortNum(0)
, GamePlaySec(0)
{
	ZeroMemory( LoginSrvIP, 15 );
	ZeroMemory( LobbySrvIP, 15 );
	ZeroMemory( GameSrvIP, 15 );
}

DataLeader::~DataLeader(void)
{
}

BOOL DataLeader::DataSetting()
{
	FILE*	fp;
	fopen_s( &fp, "SrvData.txt", "r" );

	if( fp == NULL )
		return FALSE;

	char text[256] = {0,};
	char tmpNum[10] = {0,};

	//======================================
	// Session 갯수를 받아 온다
	//======================================
	//<<Session 갯수>>
	fgets( text, 256, fp );
	//100
	fgets( text, 256, fp );
	sscanf_s( text, "%s", tmpNum, 10 );
	SessionCount = atoi( tmpNum );

	//======================================
	// 방갯수를 받아 온다
	//======================================
	//<<게임 방 갯수>>
	fgets( text, 256, fp );
	//4
	fgets( text, 256, fp );
	sscanf_s( text, "%s", tmpNum, 10 );
	RoomCount = atoi( tmpNum );

	//======================================
	// LoginSrv정보를 받아 온다
	//======================================
	//<<로그인 서버>>
	fgets( text, 256, fp );
	//192.168.0.56	8800
	fgets( text, 256, fp );
	sscanf_s( text, "%s %s", LoginSrvIP, 15, tmpNum, 10 );
	LoginSrvPortNum = atoi( tmpNum );

	//======================================
	// LobbySrv정보를 받아 온다
	//======================================
	//<<로비 서버>>
	fgets( text, 256, fp );
	//192.168.0.56	8900
	fgets( text, 256, fp );
	sscanf_s( text, "%s %s", LobbySrvIP, 15, tmpNum, 10 );
	LobbySrvPortNum = atoi( tmpNum );

	//======================================
	// GameSrv정보를 받아 온다
	//======================================
	//<<게임 서버>>
	fgets( text, 256, fp );
	//192.168.0.56	7900
	fgets( text, 256, fp );
	sscanf_s( text, "%s %s", GameSrvIP, 15, tmpNum, 10 );
	GameSrvPortNum = atoi( tmpNum );

	//======================================
	// 게임 play 시간
	//======================================
	//<<게임 play시간(초단위)>>
	fgets( text, 256, fp );
	//300
	fgets( text, 256, fp );
	sscanf_s( text, "%s", tmpNum, 10 );
	GamePlaySec = atoi( tmpNum );

	//======================================
	// LogSrv정보를 받아 온다
	//======================================
	//<<로그 서버>>
	fgets( text, 256, fp );
	//192.168.0.56	7800
	fgets( text, 256, fp );
	sscanf_s( text, "%s %s", LogSrvIP, 15, tmpNum, 10 );
	LogSrvPortNum = atoi( tmpNum );

	//======================================
	// DBSrv정보를 받아 온다
	//======================================
	//<<DB 서버>>
	fgets( text, 256, fp );
	//192.168.0.56	9000
	fgets( text, 256, fp );
	sscanf_s( text, "%s %s", DBSrvIp, 15, tmpNum, 10 );
	DBSrvPortNum = atoi( tmpNum );


	fclose( fp );

	return TRUE;
}
