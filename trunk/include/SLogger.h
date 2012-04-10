#ifndef __LOGGER__
#define __LOGGER__

//���� ũ�� 10MB
#define FILE_MAX_SIZE	100000

//���ڿ� ����
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
	// ���Ͼ��� ���� flag
	//======================================
	BOOL			canWrite;
	//==============================================================
	// ���� ���� ������
	//--------------------------------------------------------------
	//�Էµ� ���� �ӽ��̸�
	char			m_chtmpFilepath[TMPSTRING_LENTH];
	//�Էµ� ���ϰ� ���� �̸�
	char			m_chFilename[50];
	//������ file count
	int				m_fileCount;
	//�Էµ� ���� ����
	FILE			*m_pFile;
	//==============================================================
	// ������ �����ϴ� ����
	TCHAR			tstr[TMPSTRING_LENTH];
	TCHAR			tmpTStr[TMPSTRING_LENTH];		//���� �����

	char			str[TMPSTRING_LENTH];
	char			tmpStr[TMPSTRING_LENTH];		//���� �����
	char			transStr[TMPSTRING_LENTH*2];
	//==============================================================

private:
	//���ο��� ���� flag
	enum FILE_FLAG
	{
		LOG_FLAG_DEBUG		= 0x0001,
		LOG_FLAG_CONSOLE	= 0x0010,
		LOG_FLAG_FILE		= 0x0100,
	};

public:
	//����� ���� flag
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
	// ũ��Ƽ�� ����
	//======================================
	void StartCritical();
	void EndCritical();


	//���� ���� ���� ����, ����� TickTime����
	void Create( char* filename );
	//���� ���� ����
	BOOL CreateLogger();

	//���� ��µǰ� �ִ� ������ �뷮�� üũ�Ѵ�
	//�� á���� TRUE�� return�ϰ� �׷��� ������ �ٽ� �����
	//���������� ���� �Լ�
	BOOL IsFullFile();

	//������ ���ڿ��� �ܼ�â�� 

	//���ڿ��� Buf�� ����
	void PutLog( short errLv, char* lpszFmt, ... );
	void PutLog( short errLv, TCHAR* lpszFmt, ... );

	void ErrorLog( INT32 errorcode, char* lpszFmt );
	void ErrorLog( INT32 errorcode, TCHAR* lpszFmt );

	//���Ͽ� ���� �Լ�
	void WriteToFile( char* _string );

	//����
	void Release();
};

#define GetLogger SLogger::GetInstance()

#ifdef _DEBUG
#define LOGMSG	GetLogger.PutLog
#else
#define LOGMSG	__noop
#endif

#endif

