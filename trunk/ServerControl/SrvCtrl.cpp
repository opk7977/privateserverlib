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
	// 우선 주소(?)를 받아 온다
	//======================================
	::GetCurrentDirectoryA( 512, m_Dir );

	//======================================
	// DBSrv.exe
	//======================================
	char FileDir[512] = {0,};
	sprintf_s( FileDir, 512, "%s\\DBSrv.exe", m_Dir );
	if( _access( FileDir, 0 ) == -1 )
	{
		//메시지 박스 띄우기
		MessageBox( NULL, _T("DBSrv.exe를 찾지 못했습니다.\nDB서버파일의 경로를 다시 한번 확인하세요."), _T("error"), MB_OK | MB_ICONERROR );
		return FALSE;
	}

	//======================================
	// LoginSrv.exe
	//======================================
	sprintf_s( FileDir, 512, "%s\\LoginSrv.exe", m_Dir );
	if( _access( FileDir, 0 ) == -1 )
	{
		//메시지 박스 띄우기
		MessageBox( NULL, _T("LoginSrv.exe를 찾지 못했습니다.\nLogin서버파일의 경로를 다시 한번 확인하세요."), _T("error"), MB_OK | MB_ICONERROR );
		return FALSE;
	}

	//======================================
	// LobbySrv.exe
	//======================================
	sprintf_s( FileDir, 512, "%s\\LobbySrv.exe", m_Dir );
	if( _access( FileDir, 0 ) == -1 )
	{
		//메시지 박스 띄우기
		MessageBox( NULL, _T("LobbySrv.exe를 찾지 못했습니다.\nLobby서버파일의 경로를 다시 한번 확인하세요."), _T("error"), MB_OK | MB_ICONERROR );
		return FALSE;
	}

	//======================================
	// GameSrv.exe
	//======================================
	sprintf_s( FileDir, 512, "%s\\GameSrv.exe", m_Dir );
	if( _access( FileDir, 0 ) == -1 )
	{
		//메시지 박스 띄우기
		MessageBox( NULL, _T("GameSrv.exe를 찾지 못했습니다.\nGame서버파일의 경로를 다시 한번 확인하세요."), _T("error"), MB_OK | MB_ICONERROR );
		return FALSE;
	}
	
	//======================================
	// SrvData.txt
	//======================================
	sprintf_s( FileDir, 512, "%s\\Unknown.mdb", m_Dir );
	if( _access( FileDir, 0 ) == -1 )
	{
		//메시지 박스 띄우기
		MessageBox( NULL, _T("SrvData.txt를 찾지 못했습니다.\nSrvData파일의 경로를 다시 한번 확인하세요."), _T("error"), MB_OK | MB_ICONERROR );
		return FALSE;
	}

	//======================================
	// Unknown.mdb
	//======================================
	sprintf_s( FileDir, 512, "%s\\Unknown.mdb", m_Dir );
	if( _access( FileDir, 0 ) == -1 )
	{
		//메시지 박스 띄우기
		MessageBox( NULL, _T("Unknown.mdb를 찾지 못했습니다.\nUnknownDB파일의 경로를 다시 한번 확인하세요."), _T("error"), MB_OK | MB_ICONERROR );
		return FALSE;
	}

	return TRUE;
}

BOOL SrvCtrl::StartServer( char* srvName )
{
	//실행
	char tmpExeDir[512]={0,};

	//DB서버 실행
	sprintf_s( tmpExeDir, 512, "start %s\\DBSrv.exe", m_Dir );
	system( tmpExeDir );

	//Lobby서버 실행
	sprintf_s( tmpExeDir, 512, "start %s\\LobbySrv.exe", m_Dir );
	system( tmpExeDir );

	//Game서버 실행
	sprintf_s( tmpExeDir, 512, "start %s\\GameSrv.exe", m_Dir );
	system( tmpExeDir );

	//Login서버 실행
	sprintf_s( tmpExeDir, 512, "start %s\\LoginSrv.exe %s", m_Dir, srvName );
	system( tmpExeDir );

	m_isStartServer = TRUE;

	return TRUE;
}

void SrvCtrl::EndServer()
{
	//서버가 안켜져 있으면 안해도 된다
	if( !m_isStartServer )
		return;

	//종료
	char tmpExeDir[512]={0,};

	//Login서버 종료
	sprintf_s( tmpExeDir, 512, "taskkill/im LoginSrv.exe" );
	system( tmpExeDir );

	//Game서버 종료
	sprintf_s( tmpExeDir, 512, "taskkill/im GameSrv.exe" );
	system( tmpExeDir );

	//Lobby서버 종료
	sprintf_s( tmpExeDir, 512, "taskkill/im LobbySrv.exe" );
	system( tmpExeDir );

	//DB서버 종료
	sprintf_s( tmpExeDir, 512, "taskkill/im DBSrv.exe" );
	system( tmpExeDir );

	m_isStartServer = FALSE;
}
