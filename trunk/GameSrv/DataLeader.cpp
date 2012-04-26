#include "DataLeader.h"
#include "SLogger.h"


DataLeader::DataLeader(void)
: RoomCount(0)
, GameSrvProtNum(0)
, LobbySrvPortNum(0)
{
	ZeroMemory( GameSrvIP, 15 );
	ZeroMemory( LobbySrvIP, 15 );
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
	// 방갯수를 받아 온다
	//======================================
	//<<게임 방 갯수>>
	fgets( text, 256, fp );
	//4
	fgets( text, 256, fp );
	sscanf_s( text, "%s", tmpNum, 10 );
	RoomCount = atoi( tmpNum );

	//======================================
	// GameSrv정보를 받아 온다
	//======================================
	//<<게임 서버>>
	fgets( text, 256, fp );
	//192.168.0.56	7900
	fgets( text, 256, fp );
	sscanf_s( text, "%s %s", GameSrvIP, 15, tmpNum, 10 );
	GameSrvProtNum = atoi( tmpNum );

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
	// 게임 play 시간
	//======================================
	//<<게임 play시간(초단위)>>
	fgets( text, 256, fp );
	//300
	fgets( text, 256, fp );
	sscanf_s( text, "%s", tmpNum, 10 );
	GamePlaySec = atoi( tmpNum );

	fclose( fp );

	return TRUE;
}
