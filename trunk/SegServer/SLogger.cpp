#include "SLogger.h"
#include <direct.h>
#include <conio.h>
#include <time.h>
//한글 입력을 위해 사용해야 하는 헤더
#include <locale.h>

SLogger::SLogger(void) : m_pFile(0)
{
	//한글을 입력되게 선언해 줘야 한다!!!!!!!
	_tsetlocale(LC_ALL, _T("Korean")); 
	
}

SLogger::~SLogger(void)
{
#ifdef _DEBUG
	FreeConsole();
#endif

	Release();
}

void SLogger::Create( char* filename )
{
	SSynchronize Sync( this );

#ifdef _DEBUG 
	//콘솔 창을 띄움
	AllocConsole();
#endif

	//넘겨받은 filename의 길이 체크
	int filenameLen = strlen( filename );
	if( filenameLen > 255 )
	{
		OutputDebugString( _T("filename is too long\n") );
		return;
	}

	//넘겨 받은 변수 저장
	ZeroMemory( m_chFilename, filenameLen+1 );
	CopyMemory( m_chFilename, filename, filenameLen );

	//디렉토리 만든다
	_mkdir( m_chFilename );
}

BOOL SLogger::OpenFile()
{
	SSynchronize Sync( this );

	//현재 시간을 받는다.
	time_t timer = time( NULL );
	struct tm t;
	localtime_s( &t, &timer );

	//경로를 만든다.
	//파일 이름은 "제목_년월일_시분.txt"
	sprintf_s( m_chtmpFilepath, 255, "%s\\%s_%d년%d월%d일_%d시%d분.txt", m_chFilename
																		, m_chFilename
																		, t.tm_year+1900
																		, t.tm_mon+1
																		, t.tm_mday
																		, t.tm_hour
																		, t.tm_min );


	//파일을 연다( 쓰기모드 ) / 성공하면 0 return
	if( fopen_s( &m_pFile, m_chtmpFilepath, "a+" ) != 0 )
		return FALSE;

	return TRUE;
}

void SLogger::PutLog( short errLv, char* lpszFmt, ... )
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

	//-----------------------------------------------------------------
	// level에 따라 지정된 곳에 log를 남긴다.
	//-----------------------------------------------------------------
	if( errLv & LOG_FLAG_DEBUG )
	{
		//디버그 창에 한번 뿌려준다.
		OutputDebugStringA( str );
	}

#ifdef _DEBUG 
	if( errLv & LOG_FLAG_CONSOLE )
	{
		//콘솔창에 뿌림
		_cprintf_s( "%s", str );
	}
#endif

	if( errLv & LOG_FLAG_FILE )
	{
		//파일에 문자열 쓰기
		WriteToFile( str );
	}
}

void SLogger::PutLog( short errLv, TCHAR* lpszFmt, ... )
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

	//-----------------------------------------------------------------
	// level에 따라 지정된 곳에 log를 남긴다.
	//-----------------------------------------------------------------
	if( errLv & LOG_FLAG_DEBUG )
	{
		//디버그 창에 한번 뿌려준다.
		OutputDebugString( tstr );
	}

#ifdef _DEBUG
	if( errLv & LOG_FLAG_CONSOLE )
	{
		//콘솔창에 뿌림
		_cwprintf( _T("%s"), tstr );
	}
#endif

	if( errLv & LOG_FLAG_FILE )
	{
		char transStr[TMPSTRING_LENTH*2]={0,};
		WideCharToMultiByte( CP_ACP, 0, tstr, -1, transStr, _tcslen(tstr)*sizeof(TCHAR), NULL, NULL );
		//파일에 문자열 쓰기
		WriteToFile( transStr );
	}
}

void SLogger::ErrorLog( INT32 errorcode, char* lpszFmt )
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
	
	char str[TMPSTRING_LENTH]={0,};
	CopyMemory( str, lpszFmt, strlen( lpszFmt ) );
	strcat_s( str, tmpStr );

	//에러는 전부 남겨야 한다//////////////////////////////////////
	OutputDebugStringA( str );
#ifdef _DEBUG
	_cprintf_s( "%s", str );
#endif
	WriteToFile( str );
	///////////////////////////////////////////////////////////////
}

void SLogger::ErrorLog( INT32 errorcode, TCHAR* lpszFmt )
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

	TCHAR tstr[TMPSTRING_LENTH]={0,};
	CopyMemory( tstr, lpszFmt, _tcslen( lpszFmt )*sizeof(TCHAR) );
	_tcscat_s( tstr, tmpTStr );

	//에러는 전부 남겨야 한다//////////////////////////////////////
	OutputDebugString( tstr );
#ifdef _DEBUG
	_cwprintf( _T("%s\n"), tstr );
#endif
	char transStr[TMPSTRING_LENTH*2]={0,};
	WideCharToMultiByte( CP_ACP, 0, tstr, -1, transStr, _tcslen(tstr)*sizeof(TCHAR), NULL, NULL );
	WriteToFile( transStr );
	///////////////////////////////////////////////////////////////
}

void SLogger::WriteToFile( char* _string )
{
	SSynchronize Sync( this );

	if( !OpenFile() )
	{
		OutputDebugStringA( "파일을 열 수 없습니다.\n\n" );
		return;
	}

	//문자열을 파일에 저장
	fwrite( _string, strlen( _string ), 1, m_pFile );

	Release();
}

void SLogger::Release()
{
	if( m_pFile == 0 )
		return;

	fclose( m_pFile );
	m_pFile = 0;
}
