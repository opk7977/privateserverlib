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
	// �氹���� �޾� �´�
	//======================================
	//<<���� �� ����>>
	fgets( text, 256, fp );
	//4
	fgets( text, 256, fp );
	sscanf_s( text, "%s", tmpNum, 10 );
	RoomCount = atoi( tmpNum );

	//======================================
	// GameSrv������ �޾� �´�
	//======================================
	//<<���� ����>>
	fgets( text, 256, fp );
	//192.168.0.56	7900
	fgets( text, 256, fp );
	sscanf_s( text, "%s %s", GameSrvIP, 15, tmpNum, 10 );
	GameSrvProtNum = atoi( tmpNum );

	//======================================
	// LobbySrv������ �޾� �´�
	//======================================
	//<<�κ� ����>>
	fgets( text, 256, fp );
	//192.168.0.56	8900
	fgets( text, 256, fp );
	sscanf_s( text, "%s %s", LobbySrvIP, 15, tmpNum, 10 );
	LobbySrvPortNum = atoi( tmpNum );

	//======================================
	// ���� play �ð�
	//======================================
	//<<���� play�ð�(�ʴ���)>>
	fgets( text, 256, fp );
	//300
	fgets( text, 256, fp );
	sscanf_s( text, "%s", tmpNum, 10 );
	GamePlaySec = atoi( tmpNum );

	fclose( fp );

	return TRUE;
}
