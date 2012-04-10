#include "SLogger.h"
#include <direct.h>
#include <conio.h>
#include <time.h>
//한글 입력을 위해 사용해야 하는 헤더
#include <locale.h>

SLogger::SLogger(void) : m_fileCount(0)
					   , canWrite(FALSE)
					   , m_pFile(NULL)
{
	m_logCritical = new CRITICAL_SECTION;
	InitializeCriticalSection( m_logCritical );

	//기본적으로 사용될 파일 하나를 생성
	Create( "Server" );

	//한글을 입력되게 선언해 줘야 한다!!!!!!!
	_tsetlocale(LC_ALL, _T("Korean")); 

	//콘솔 창을 띄움
	AllocConsole();
}

SLogger::~SLogger(void)
{
	Release();

	DeleteCriticalSection( m_logCritical );
	delete m_logCritical;
	m_logCritical = 0;

	FreeConsole();
}

void SLogger::StartCritical()
{
	if( m_logCritical == NULL )
		return;

	EnterCriticalSection( m_logCritical );
}

void SLogger::EndCritical()
{
	if( m_logCritical == NULL )
		return;

	LeaveCriticalSection( m_logCritical );
}

void SLogger::Create( char* filename )
{
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

	//디렉토리 만들고
	_mkdir( m_chFilename );

	//log파일을 만든다
	CreateLogger();
}

BOOL SLogger::CreateLogger()
{
	StartCritical();

	//우선 글쓰기를 막아 두고
	canWrite = FALSE;

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


	//이미 파일이 열려 있다면 닫고
	if( m_pFile != 0 )
		Release();

	//파일을 연다( 쓰기모드 ) / 성공하면 0 return
	int retval = fopen_s( &m_pFile, m_chtmpFilepath, "a+" );

	if( retval != 0 )
		return FALSE;

	//다시 쓸수 있게 풀어 준다.
	canWrite = TRUE;
	EndCritical();

	return TRUE;
}

BOOL SLogger::IsFullFile()
{
	fseek(m_pFile, 0L, SEEK_END);
	int fileLen = ftell( m_pFile );

	//현재 파일의 크기가 FILE_MAX_SIZE를 넘었다면
	if( fileLen >= FILE_MAX_SIZE )
		return TRUE;

	return FALSE;
}

void SLogger::PutLog( short errLv, char* lpszFmt, ... )
{
	StartCritical();

	ZeroMemory( str, TMPSTRING_LENTH );
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
	if( errLv & LOG_FLAG_CONSOLE )
	{
		//콘솔창에 뿌림
		_cprintf_s( "%s", str );
	}
	if( errLv & LOG_FLAG_FILE )
	{
		//파일에 문자열 쓰기
		WriteToFile( str );
	}

	EndCritical();
}

void SLogger::PutLog( short errLv, TCHAR* lpszFmt, ... )
{
	StartCritical();

	ZeroMemory( tstr, TMPSTRING_LENTH * sizeof(TCHAR) );
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
	if( errLv & LOG_FLAG_CONSOLE )
	{
		//콘솔창에 뿌림
		_cwprintf( _T("%s"), tstr );
	}
	if( errLv & LOG_FLAG_FILE )
	{
		WideCharToMultiByte( CP_ACP, 0, tstr, -1, transStr, _tcslen(tstr)*sizeof(TCHAR), NULL, NULL );
		//파일에 문자열 쓰기
		WriteToFile( transStr );
	}

	EndCritical();
}

void SLogger::ErrorLog( INT32 errorcode, char* lpszFmt )
{
	StartCritical();

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

	ZeroMemory( tstr, TMPSTRING_LENTH );
	CopyMemory( str, lpszFmt, strlen( lpszFmt ) );
	strcat_s( str, tmpStr );

	//에러는 전부 남겨야 한다//////////////////////////////////////
	OutputDebugStringA( str );
	_cprintf_s( "%s", str );
	WriteToFile( str );
	///////////////////////////////////////////////////////////////

	EndCritical();

}

void SLogger::ErrorLog( INT32 errorcode, TCHAR* lpszFmt )
{
	StartCritical();

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

	ZeroMemory( tstr, TMPSTRING_LENTH );
	CopyMemory( tstr, lpszFmt, _tcslen( lpszFmt )*sizeof(TCHAR) );
	_tcscat_s( tstr, tmpTStr );

	//에러는 전부 남겨야 한다//////////////////////////////////////
	OutputDebugString( tstr );
	_cwprintf( _T("%s"), tstr );
	WideCharToMultiByte( CP_ACP, 0, tstr, -1, transStr, _tcslen(tstr)*sizeof(TCHAR), NULL, NULL );
	WriteToFile( transStr );
	///////////////////////////////////////////////////////////////

	EndCritical();
}

void SLogger::WriteToFile( char* _string )
{
	if( !canWrite )
	{
		//현재 파일은 닫혀 있는 상태라면 디버그창에만 뿌려야 한다.
		//넘어 가자
		OutputDebugStringA( "파일이 현재 닫혀 있습니다\n파일에는 적히지 않았습니다.\n" );
		return;
	}

	//현재 파일의 크기를 확인하고 넘어 갔다면
	//새로 파일을 생성해 주고 파일에 쓰기 작업을 해야 한다. 
	if( IsFullFile() )	//넘었으면 새로 파일을 만들어야 함
		CreateLogger();

	//문자열을 파일에 저장
	fwrite( _string, strlen( _string ), 1, m_pFile );
}

void SLogger::Release()
{
	canWrite = FALSE;

	fclose( m_pFile );
	m_pFile = 0;
}
