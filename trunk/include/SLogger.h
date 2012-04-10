#ifndef __LOGGER__
#define __LOGGER__

//파일 크기 10MB
#define FILE_MAX_SIZE	100000

//문자열 길이
#define TMPSTRING_LENTH		1024

#include "SServerStd.h"

class SLogger : public SSingleton <SLogger>
{
private:
	friend class SSingleton<SLogger>;

private:
	CRITICAL_SECTION*	m_logCritical;

private:
	//======================================
	// 파일쓰기 가능 flag
	//======================================
	BOOL			canWrite;
	//==============================================================
	// 파일 관련 변수들
	//--------------------------------------------------------------
	//입력될 파일 임시이름
	char			m_chtmpFilepath[TMPSTRING_LENTH];
	//입력될 파일과 폴더 이름
	char			m_chFilename[50];
	//생성된 file count
	int				m_fileCount;
	//입력될 파일 공간
	FILE			*m_pFile;
	//==============================================================
	// 문장을 저장하는 변수
	TCHAR			tstr[TMPSTRING_LENTH];
	TCHAR			tmpTStr[TMPSTRING_LENTH];		//에러 추출용

	char			str[TMPSTRING_LENTH];
	char			tmpStr[TMPSTRING_LENTH];		//에러 추출용
	char			transStr[TMPSTRING_LENTH*2];
	//==============================================================

private:
	//내부에서 비교할 flag
	enum FILE_FLAG
	{
		LOG_FLAG_DEBUG		= 0x0001,
		LOG_FLAG_CONSOLE	= 0x0010,
		LOG_FLAG_FILE		= 0x0100,
	};

public:
	//사용자 지정 flag
	enum FILE_LEVEL
	{
		LOG_LEVEL_DBGINFO	= 0x0001,
		LOG_LEVEL_CONINFO	= 0x0010,
		LOG_LEVEL_SYSTEM	= 0x0011,
		LOG_LEVEL_WORRNIG	= 0x0111,
	};


private:
	SLogger(void);
	virtual ~SLogger(void);

public:
	//======================================
	// 크리티컬 섹션
	//======================================
	void StartCritical();
	void EndCritical();


	//파일 저장 공간 생성, 저장될 TickTime셋팅
	void Create( char* filename );
	//저장 파일 생성
	BOOL CreateLogger();

	//현재 출력되고 있는 파일의 용량을 체크한다
	//다 찼으면 TRUE를 return하고 그러면 파일을 다시 만든다
	//내부적으로 사용될 함수
	BOOL IsFullFile();

	//간단한 문자열은 콘솔창에 

	//문자열을 Buf에 저장
	void PutLog( short errLv, char* lpszFmt, ... );
	void PutLog( short errLv, TCHAR* lpszFmt, ... );

	void ErrorLog( INT32 errorcode, char* lpszFmt );
	void ErrorLog( INT32 errorcode, TCHAR* lpszFmt );

	//파일에 쓰는 함수
	void WriteToFile( char* _string );

	//해제
	void Release();
};

#define GetLogger SLogger::GetInstance()

#ifdef _DEBUG
#define LOGMSG	GetLogger.PutLog
#else
#define LOGMSG	__noop
#endif

#endif

