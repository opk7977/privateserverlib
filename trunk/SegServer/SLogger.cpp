#include "SLogger.h"
#include <direct.h>
#include <conio.h>
#include <time.h>
//�ѱ� �Է��� ���� ����ؾ� �ϴ� ���
#include <locale.h>

SLogger::SLogger(void) : m_pFile(0)
{
	//�ѱ��� �Էµǰ� ������ ��� �Ѵ�!!!!!!!
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
	//�ܼ� â�� ���
	AllocConsole();
#endif

	//�Ѱܹ��� filename�� ���� üũ
	int filenameLen = strlen( filename );
	if( filenameLen > 255 )
	{
		OutputDebugString( _T("filename is too long\n") );
		return;
	}

	//�Ѱ� ���� ���� ����
	ZeroMemory( m_chFilename, filenameLen+1 );
	CopyMemory( m_chFilename, filename, filenameLen );

	//���丮 �����
	_mkdir( m_chFilename );
}

BOOL SLogger::OpenFile()
{
	SSynchronize Sync( this );

	//���� �ð��� �޴´�.
	time_t timer = time( NULL );
	struct tm t;
	localtime_s( &t, &timer );

	//��θ� �����.
	//���� �̸��� "����_�����_�ú�.txt"
	sprintf_s( m_chtmpFilepath, 255, "%s\\%s_%d��%d��%d��_%d��%d��.txt", m_chFilename
																		, m_chFilename
																		, t.tm_year+1900
																		, t.tm_mon+1
																		, t.tm_mday
																		, t.tm_hour
																		, t.tm_min );


	//������ ����( ������ ) / �����ϸ� 0 return
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
	// �������ڸ� �ٷ� �޾� ���ڿ� �������� ..
	//-----------------------------------------------------------------
	va_start(Vargs, lpszFmt);
	vsprintf_s( str, lpszFmt, Vargs );
	va_end(Vargs);

	//-----------------------------------------------------------------
	// level�� ���� ������ ���� log�� �����.
	//-----------------------------------------------------------------
	if( errLv & LOG_FLAG_DEBUG )
	{
		//����� â�� �ѹ� �ѷ��ش�.
		OutputDebugStringA( str );
	}

#ifdef _DEBUG 
	if( errLv & LOG_FLAG_CONSOLE )
	{
		//�ܼ�â�� �Ѹ�
		_cprintf_s( "%s", str );
	}
#endif

	if( errLv & LOG_FLAG_FILE )
	{
		//���Ͽ� ���ڿ� ����
		WriteToFile( str );
	}
}

void SLogger::PutLog( short errLv, TCHAR* lpszFmt, ... )
{
	SSynchronize Sync( this );

	TCHAR tstr[TMPSTRING_LENTH]={0,};
	va_list Vargs;

	//-----------------------------------------------------------------
	// �������ڸ� �ٷ� �޾� ���ڿ� �������� ..
	//-----------------------------------------------------------------
	va_start(Vargs, lpszFmt);
	vswprintf_s( tstr, lpszFmt, Vargs );
	va_end(Vargs);

	//-----------------------------------------------------------------
	// level�� ���� ������ ���� log�� �����.
	//-----------------------------------------------------------------
	if( errLv & LOG_FLAG_DEBUG )
	{
		//����� â�� �ѹ� �ѷ��ش�.
		OutputDebugString( tstr );
	}

#ifdef _DEBUG
	if( errLv & LOG_FLAG_CONSOLE )
	{
		//�ܼ�â�� �Ѹ�
		_cwprintf( _T("%s"), tstr );
	}
#endif

	if( errLv & LOG_FLAG_FILE )
	{
		char transStr[TMPSTRING_LENTH*2]={0,};
		WideCharToMultiByte( CP_ACP, 0, tstr, -1, transStr, _tcslen(tstr)*sizeof(TCHAR), NULL, NULL );
		//���Ͽ� ���ڿ� ����
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

	//������ ���� ���ܾ� �Ѵ�//////////////////////////////////////
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

	//������ ���� ���ܾ� �Ѵ�//////////////////////////////////////
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
		OutputDebugStringA( "������ �� �� �����ϴ�.\n\n" );
		return;
	}

	//���ڿ��� ���Ͽ� ����
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
