#include "DataLeader.h"

DataLeader::DataLeader(void)
: SessionCount(0)
, RoomCount(0)
, LoginSrvPortNum(0)
, LobbySrvPortNum(0)
, GameSrvPortNum(0)
, LogSrvPortNum(0)
, DBSrvPortNum(0)
{
	ZeroMemory( LoginSrvIP, 15 );
	ZeroMemory( LobbySrvIP, 15 );
	ZeroMemory( GameSrvIP, 15 );
	ZeroMemory( LogSrvIP, 15 );
	ZeroMemory( DBSrvIp, 15 );
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

	//==============================================================
	//======================================
	// DB 연결정보를 받는다.
	//======================================
	//<<DB connect mdbFile:0 / SQLServer2008DB:1>>
	fgets( text, 256, fp );
	//1
	fgets( text, 256, fp );
	//======================================
	// DB이름을 받는다
	//======================================
	//<<DB NAME>>
	fgets( text, 256, fp );
	//Unknown
	fgets( text, 256, fp );
	//연결형 서버가 아니라면 뒤의 정보는 읽을 필요 없음
	//======================================
	// DB Srv정보를 받는다
	//======================================
	//<<DB Server ID/ Password>>
	fgets( text, 256, fp );
	//sa 1234
	fgets( text, 256, fp );
	//==============================================================

	//======================================
	// 자신이 여는 서버의 제목
	//======================================
	//<<SrvName_게임서버 열때 제목>>
	fgets( text, 256, fp );
	//Unknown 
	fgets( text, 256, fp );
	char tmpSrvname[32]={0,};
	sscanf_s( text, "%s", tmpSrvname, 32 );
 	ZeroMemory( SrvName, 32 );
	MultiByteToWideChar( CP_ACP, 0, tmpSrvname, -1, SrvName, 32 );

	fclose( fp );

	return TRUE;
}
