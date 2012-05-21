#include "LogMgr.h"
#include "LogSrvNet.h"
#include "LoginProtocol.h"
#include "SPacket.h"

#include "LogSrvNet.h"

//한글 입력을 위해 사용해야 하는 헤더
#include <locale.h>

LogMgr::LogMgr(void)
{
	//한글을 입력되게 선언해 줘야 한다!!!!!!!
	_tsetlocale(LC_ALL, _T("Korean")); 

	m_logSrv = &GetSrvNet;
}

LogMgr::~LogMgr(void)
{
}

void LogMgr::SendLog( int SrvID, int errLv, char* lpszFmt, ... )
{
	SSynchronize Sync( this );

	char str[TMPSTRING_LENTH]={0,};
	va_list Vargs;

	//-----------------------------------------------------------------
	// 가변인자를 바로 받아 문자열 형식으로 ..
	//-----------------------------------------------------------------
	va_start(Vargs, lpszFmt);
	vsprintf_s( str, lpszFmt, Vargs );
	va_end(Vargs);

	TCHAR tmptStr[512]={0,};
	MultiByteToWideChar( CP_ACP, 0, str, -1, tmptStr, strlen(str) );

	SendPacket( SrvID, errLv, tmptStr );	
}

void LogMgr::SendLog( int SrvID, int errLv, TCHAR* lpszFmt, ... )
{
	SSynchronize Sync( this );

	TCHAR tstr[TMPSTRING_LENTH]={0,};
	va_list Vargs;

	//-----------------------------------------------------------------
	// 가변인자를 바로 받아 문자열 형식으로 ..
	//-----------------------------------------------------------------
	va_start(Vargs, lpszFmt);
	vswprintf_s( tstr, lpszFmt, Vargs );
	va_end(Vargs);

	SendPacket( SrvID, errLv, tstr );
}

void LogMgr::SendErrLog( int SrvID, INT32 errorcode, char* lpszFmt )
{
	SSynchronize Sync( this );

	char tmpStr[TMPSTRING_LENTH]={0,};

	int retval = FormatMessageA( FORMAT_MESSAGE_FROM_SYSTEM
								, NULL
								, errorcode
								, 0
								, (LPSTR)&tmpStr
								, TMPSTRING_LENTH
								, NULL );

	if( retval == 0 )
	{
		OutputDebugStringA( "Filed Get Error Meg\n" );
		return;
	}

	TCHAR tmptStr[512]={0,};
	MultiByteToWideChar( CP_ACP, 0, tmpStr, -1, tmptStr, strlen(tmpStr) );

	SendPacket( SrvID, LOG_LEVEL_ERROR, tmptStr );	
}

void LogMgr::SendErrLog( int SrvID, INT32 errorcode, TCHAR* lpszFmt )
{
	SSynchronize Sync( this );

	TCHAR tmpTStr[TMPSTRING_LENTH]={0,};

	int retval = FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM
								, NULL
								, errorcode
								, 0
								, (LPWSTR)&tmpTStr
								, TMPSTRING_LENTH
								, NULL );

	if( retval == 0 )
	{
		OutputDebugStringW( _T("Filed Get Error Meg\n") );
		return;
	}

	SendPacket( SrvID, LOG_LEVEL_ERROR, tmpTStr );
}

void LogMgr::SendPacket( int SrvID, int LogLv, TCHAR* stringLog )
{
	SPacket sendPacket( RECV_SERVER_LOG );
	sendPacket << SrvID;
	sendPacket << LogLv;

	//문장 입력
	int size = _tcslen( stringLog ) * sizeof( TCHAR );
	sendPacket << size;
	sendPacket.PutData( stringLog, size );

	m_logSrv->SendToLogServer( sendPacket );
}
