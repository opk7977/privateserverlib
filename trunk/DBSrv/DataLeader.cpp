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
	ZeroMemory( DBName, 15 );
	ZeroMemory( DBSrvID, 15 );
	ZeroMemory( DBSrvPW, 15 );
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
	// Session ������ �޾� �´�
	//======================================
	//<<Session ����>>
	fgets( text, 256, fp );
	//100
	fgets( text, 256, fp );
	sscanf_s( text, "%s", tmpNum, 10 );
	SessionCount = atoi( tmpNum );

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
	// LoginSrv������ �޾� �´�
	//======================================
	//<<�α��� ����>>
	fgets( text, 256, fp );
	//192.168.0.56	8800
	fgets( text, 256, fp );
	sscanf_s( text, "%s %s", LoginSrvIP, 15, tmpNum, 10 );
	LoginSrvPortNum = atoi( tmpNum );

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
	// GameSrv������ �޾� �´�
	//======================================
	//<<���� ����>>
	fgets( text, 256, fp );
	//192.168.0.56	7900
	fgets( text, 256, fp );
	sscanf_s( text, "%s %s", GameSrvIP, 15, tmpNum, 10 );
	GameSrvPortNum = atoi( tmpNum );

	//======================================
	// LogSrv������ �޾� �´�
	//======================================
	//<<�α� ����>>
	fgets( text, 256, fp );
	//192.168.0.56	7800
	fgets( text, 256, fp );
	sscanf_s( text, "%s %s", LogSrvIP, 15, tmpNum, 10 );
	LogSrvPortNum = atoi( tmpNum );

	//======================================
	// DBSrv������ �޾� �´�
	//======================================
	//<<DB ����>>
	fgets( text, 256, fp );
	//192.168.0.56	9000
	fgets( text, 256, fp );
	sscanf_s( text, "%s %s", DBSrvIp, 15, tmpNum, 10 );
	DBSrvPortNum = atoi( tmpNum );

	//======================================
	// DB ���������� �޴´�.
	//======================================
	//<<DB connect mdbFile:0 / SQLServer2008DB:1>>
	fgets( text, 256, fp );
	//1
	fgets( text, 256, fp );
	sscanf_s( text, "%s", tmpNum, 10 );
	isConnectSrv = (BOOL)atoi( tmpNum );

	//======================================
	// DB�̸��� �޴´�
	//======================================
	//<<DB NAME>>
	fgets( text, 256, fp );
	//Unknown
	fgets( text, 256, fp );
	char tmpDBname[15]={0,};
	sscanf_s( text, "%s", tmpDBname, 15 );
	MultiByteToWideChar( CP_ACP, 0, tmpDBname, -1, DBName, 15 );

	//������ ������ �ƴ϶�� ���� ������ ���� �ʿ� ����
	if( isConnectSrv )
	{
		//======================================
		// DB Srv������ �޴´�
		//======================================
		//<<DB Server ID/ Password>>
		fgets( text, 256, fp );
		//sa 1234
		fgets( text, 256, fp );
		char tmpId[15] = {0,};
		char tmpPw[15] = {0,};
		sscanf_s( text, "%s %s", tmpId, 15, tmpPw, 15 );
		MultiByteToWideChar( CP_ACP, 0, tmpId, -1, DBSrvID, 15 );
		MultiByteToWideChar( CP_ACP, 0, tmpPw, -1, DBSrvPW, 15 );
	}		

	fclose( fp );

	return TRUE;
}
