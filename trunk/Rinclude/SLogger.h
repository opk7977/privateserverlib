#ifndef __LOGGER__
#define __LOGGER__

//파일 크기 10MB
#define FILE_MAX_SIZE	100000

//문자열 길이
#define TMPSTRING_LENTH		1024

#include "SServerObj.h"

class SLogger : public SSingleton <SLogger>, public SServerObj
{
private:
	friend class SSingleton<SLogger>;

private:
	//==============================================================
	// 파일 관련 변수들
	//--------------------------------------------------------------
	//입력될 파일 임시이름
	char			m_chtmpFilepath[TMPSTRING_LENTH];
	//입력될 파일과 폴더 이름
	char			m_chFilename[50];
	//입력될 파일 공간
	FILE			*m_pFile;
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

	//파일 저장 공간 생성, 저장될 TickTime셋팅
	void Create( char* filename );
	//저장 파일 열기
	BOOL OpenFile();

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

#endif

