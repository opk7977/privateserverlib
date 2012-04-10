#include "SLogger.h"
#include <direct.h>
#include <conio.h>
#include <time.h>
//�ѱ� �Է��� ���� ����ؾ� �ϴ� ���
#include <locale.h>

SLogger::SLogger(void) : m_fileCount(0)
					   , canWrite(FALSE)
					   , m_pFile(NULL)
{
	m_logCritical = new CRITICAL_SECTION;
	InitializeCriticalSection( m_logCritical );

	//�⺻������ ���� ���� �ϳ��� ����
	Create( "Server" );

	//�ѱ��� �Էµǰ� ������ ��� �Ѵ�!!!!!!!
	_tsetlocale(LC_ALL, _T("Korean")); 

	//�ܼ� â�� ���
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

	//log������ �����
	CreateLogger();
}

BOOL SLogger::CreateLogger()
{
	StartCritical();

	//�켱 �۾��⸦ ���� �ΰ�
	canWrite = FALSE;

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


	//�̹� ������ ���� �ִٸ� �ݰ�
	if( m_pFile != 0 )
		Release();

	//������ ����( ������ ) / �����ϸ� 0 return
	int retval = fopen_s( &m_pFile, m_chtmpFilepath, "a+" );

	if( retval != 0 )
		return FALSE;

	//�ٽ� ���� �ְ� Ǯ�� �ش�.
	canWrite = TRUE;
	EndCritical();

	return TRUE;
}

BOOL SLogger::IsFullFile()
{
	fseek(m_pFile, 0L, SEEK_END);
	int fileLen = ftell( m_pFile );

	//���� ������ ũ�Ⱑ FILE_MAX_SIZE�� �Ѿ��ٸ�
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
	if( errLv & LOG_FLAG_CONSOLE )
	{
		//�ܼ�â�� �Ѹ�
		_cprintf_s( "%s", str );
	}
	if( errLv & LOG_FLAG_FILE )
	{
		//���Ͽ� ���ڿ� ����
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
	if( errLv & LOG_FLAG_CONSOLE )
	{
		//�ܼ�â�� �Ѹ�
		_cwprintf( _T("%s"), tstr );
	}
	if( errLv & LOG_FLAG_FILE )
	{
		WideCharToMultiByte( CP_ACP, 0, tstr, -1, transStr, _tcslen(tstr)*sizeof(TCHAR), NULL, NULL );
		//���Ͽ� ���ڿ� ����
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

	//������ ���� ���ܾ� �Ѵ�//////////////////////////////////////
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

	//������ ���� ���ܾ� �Ѵ�//////////////////////////////////////
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
		//���� ������ ���� �ִ� ���¶�� �����â���� �ѷ��� �Ѵ�.
		//�Ѿ� ����
		OutputDebugStringA( "������ ���� ���� �ֽ��ϴ�\n���Ͽ��� ������ �ʾҽ��ϴ�.\n" );
		return;
	}

	//���� ������ ũ�⸦ Ȯ���ϰ� �Ѿ� ���ٸ�
	//���� ������ ������ �ְ� ���Ͽ� ���� �۾��� �ؾ� �Ѵ�. 
	if( IsFullFile() )	//�Ѿ����� ���� ������ ������ ��
		CreateLogger();

	//���ڿ��� ���Ͽ� ����
	fwrite( _string, strlen( _string ), 1, m_pFile );
}

void SLogger::Release()
{
	canWrite = FALSE;

	fclose( m_pFile );
	m_pFile = 0;
}
