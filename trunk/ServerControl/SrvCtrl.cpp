#include "SrvCtrl.h"
#include <io.h>

SrvCtrl::SrvCtrl(void)
: m_isStartServer(FALSE)
{
}

SrvCtrl::~SrvCtrl(void)
{
}

BOOL SrvCtrl::CheckFindNeedAllFile()
{
	//======================================
	// DBSrv.exe
	//======================================
	if( _access( "DBSrv.exe", 0 ) == -1 )
	{
		//�޽��� �ڽ� ����
		MessageBox( NULL, _T("DBSrv.exe�� ã�� ���߽��ϴ�.\nDB���������� ��θ� �ٽ� �ѹ� Ȯ���ϼ���."), _T("error"), MB_OK | MB_ICONERROR );
		return FALSE;
	}

	//======================================
	// LoginSrv.exe
	//======================================
	if( _access( "LoginSrv.exe", 0 ) == -1 )
	{
		//�޽��� �ڽ� ����
		MessageBox( NULL, _T("LoginSrv.exe�� ã�� ���߽��ϴ�.\nLogin���������� ��θ� �ٽ� �ѹ� Ȯ���ϼ���."), _T("error"), MB_OK | MB_ICONERROR );
		return FALSE;
	}

	//======================================
	// LobbySrv.exe
	//======================================
	if( _access( "LobbySrv.exe", 0 ) == -1 )
	{
		//�޽��� �ڽ� ����
		MessageBox( NULL, _T("LobbySrv.exe�� ã�� ���߽��ϴ�.\nLobby���������� ��θ� �ٽ� �ѹ� Ȯ���ϼ���."), _T("error"), MB_OK | MB_ICONERROR );
		return FALSE;
	}

	//======================================
	// GameSrv.exe
	//======================================
	if( _access( "GameSrv.exe", 0 ) == -1 )
	{
		//�޽��� �ڽ� ����
		MessageBox( NULL, _T("GameSrv.exe�� ã�� ���߽��ϴ�.\nGame���������� ��θ� �ٽ� �ѹ� Ȯ���ϼ���."), _T("error"), MB_OK | MB_ICONERROR );
		return FALSE;
	}

	//======================================
	// SrvData.txt
	//======================================
	if( _access( "Unknown.mdb", 0 ) == -1 )
	{
		//�޽��� �ڽ� ����
		MessageBox( NULL, _T("SrvData.txt�� ã�� ���߽��ϴ�.\nSrvData������ ��θ� �ٽ� �ѹ� Ȯ���ϼ���."), _T("error"), MB_OK | MB_ICONERROR );
		return FALSE;
	}

	//======================================
	// Unknown.mdb
	//======================================
	if( _access( "Unknown.mdb", 0 ) == -1 )
	{
		//�޽��� �ڽ� ����
		MessageBox( NULL, _T("Unknown.mdb�� ã�� ���߽��ϴ�.\nUnknownDB������ ��θ� �ٽ� �ѹ� Ȯ���ϼ���."), _T("error"), MB_OK | MB_ICONERROR );
		return FALSE;
	}

	return TRUE;
}

BOOL SrvCtrl::StartServer( char* srvName )
{
	//����
	char tmpExeDir[512]={0,};

	//DB���� ����
	sprintf_s( tmpExeDir, 512, "start DBSrv.exe" );
	system( tmpExeDir );

	//Lobby���� ����
	sprintf_s( tmpExeDir, 512, "start LobbySrv.exe" );
	system( tmpExeDir );

	//Game���� ����
	sprintf_s( tmpExeDir, 512, "start GameSrv.exe" );
	system( tmpExeDir );

	//Login���� ����
	sprintf_s( tmpExeDir, 512, "start LoginSrv.exe %s", srvName );
	system( tmpExeDir );

	m_isStartServer = TRUE;

	return TRUE;
}

void SrvCtrl::EndServer()
{
	//������ ������ ������ ���ص� �ȴ�
	if( !m_isStartServer )
		return;

	//����
	char tmpExeDir[512]={0,};

	//Login���� ����
	sprintf_s( tmpExeDir, 512, "taskkill/im LoginSrv.exe" );
	system( tmpExeDir );

	//Game���� ����
	sprintf_s( tmpExeDir, 512, "taskkill/im GameSrv.exe" );
	system( tmpExeDir );

	//Lobby���� ����
	sprintf_s( tmpExeDir, 512, "taskkill/im LobbySrv.exe" );
	system( tmpExeDir );

	//DB���� ����
	sprintf_s( tmpExeDir, 512, "taskkill/im DBSrv.exe" );
	system( tmpExeDir );

	m_isStartServer = FALSE;
}
