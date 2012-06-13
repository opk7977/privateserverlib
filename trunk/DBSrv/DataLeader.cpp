#include "DataLeader.h"

DataLeader::DataLeader(void)
: SessionCount(0)
, LogSrvPortNum(0)
{
	ZeroMemory( LogSrvIP, 15 );
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
	// LogSrv정보를 받아 온다
	//======================================
	//<<로그 서버>>
	fgets( text, 256, fp );
	//192.168.0.56	7800
	fgets( text, 256, fp );
	sscanf_s( text, "%s %s", LogSrvIP, 15, tmpNum, 10 );
	LogSrvPortNum = atoi( tmpNum );

	fclose( fp );

	return TRUE;
}
