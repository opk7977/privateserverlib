#include "DataLeader.h"

DataLeader::DataLeader(void)
: SessionCount(0)
, DBSrvPortNum(0)
, LoginSrvPortNum(0)
, LobbySrvPortNum(0)
, GameSrvPortNum(0)
, LogSrvPortNum(0)
{
	ZeroMemory( DBSrvIp, 15 );
	ZeroMemory( LoginSrvIP, 15 );
	ZeroMemory( LobbySrvIP, 15 );
	ZeroMemory( GameSrvIP, 15 );
	ZeroMemory( LogSrvIP, 15 );
}

DataLeader::~DataLeader(void)
{
}

BOOL DataLeader::DataSetting()
{
	//==============================================================
	// ���Ͽ��� �о� ���� data
	//==============================================================

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
	// ���������� �ڵ����� �Ұ���
	// �о� �鿩 �Ұ��� üũ
	//======================================
	//<<AutoServerSetting TRUE(=1)/ FALSE(=0)>>
	fgets( text, 256, fp );
	//1
	fgets( text, 256, fp );
	sscanf_s( text, "%s", tmpNum, 10 );
	isAutoSrvSetting = (BOOL)atoi( tmpNum );

	if( isAutoSrvSetting )
	{
		AutoServerSetting();
	}
	else
	{
		//======================================
		// DBSrv������ �޾� �´�
		//======================================
		//<<DB ����>>
		fgets( text, 256, fp );
		//192.168.0.56	9000
		fgets( text, 256, fp );
		sscanf_s( text, "%s", DBSrvIp, 15 );

		//======================================
		// LoginSrv������ �޾� �´�
		//======================================
		//<<�α��� ����>>
		fgets( text, 256, fp );
		//192.168.0.56	8800
		fgets( text, 256, fp );
		sscanf_s( text, "%s", LoginSrvIP, 15 );

		//======================================
		// LobbySrv������ �޾� �´�
		//======================================
		//<<�κ� ����>>
		fgets( text, 256, fp );
		//192.168.0.56	8900
		fgets( text, 256, fp );
		sscanf_s( text, "%s", LobbySrvIP, 15 );

		//======================================
		// GameSrv������ �޾� �´�
		//======================================
		//<<���� ����>>
		fgets( text, 256, fp );
		//192.168.0.56	7900
		fgets( text, 256, fp );
		sscanf_s( text, "%s", GameSrvIP, 15 );

		//======================================
		// LogSrv������ �޾� �´�
		//======================================
		//<<�α� ����>>
		fgets( text, 256, fp );
		//192.168.0.56	7800
		fgets( text, 256, fp );
		sscanf_s( text, "%s", LogSrvIP, 15 );
	}

	fclose( fp );

	//==============================================================
	// �⺻���� ������ �ִ� data
	//==============================================================
	//--------------------------------------
	// ��Ʈ��ȣ ����
	//--------------------------------------
	DBSrvPortNum		= DBSRV_PORTNUM;
	LoginSrvPortNum		= LOGINSRV_PORTNUM;
	LobbySrvPortNum		= LOBBYSRV_PORTNUM;
	GameSrvPortNum		= GAMESRV_PORTNUM;
	LogSrvPortNum		= LOGSRV_PORTNUM;

	return TRUE;
}

void DataLeader::AutoServerSetting()
{
	hostent* thisHost;
	char* ip;
	char szHostName[40];
	WSADATA wsaData;
	BOOL bRtnVal = FALSE;

	if( WSAStartup(MAKEWORD(2,2), &wsaData) == NO_ERROR )
	{
		//���� ȣ��Ʈ�� �̸��� ��´�.
		int nResult = gethostname( szHostName, 40 );
		if( nResult == 0 && strcmp(szHostName, "") != 0 )
		{
			// ȣ��Ʈ ���� ������ ã�´�.
			if( (thisHost = gethostbyname(szHostName)) != NULL )
			{
				ip = inet_ntoa (*(struct in_addr *)*thisHost->h_addr_list);
				sprintf_s( DBSrvIp, sizeof( DBSrvIp ), ip );
				sprintf_s( LoginSrvIP, sizeof( LoginSrvIP ), ip );
				sprintf_s( LobbySrvIP, sizeof( LobbySrvIP ), ip );
				sprintf_s( GameSrvIP, sizeof( GameSrvIP ), ip );
				sprintf_s( LogSrvIP, sizeof( LogSrvIP ), ip );
			}
		}
		WSACleanup();
	}
}


