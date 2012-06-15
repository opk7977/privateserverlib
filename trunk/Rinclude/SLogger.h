#ifndef __LOGGER__
#define __LOGGER__

//���� ũ�� 10MB
#define FILE_MAX_SIZE	100000

//���ڿ� ����
#define TMPSTRING_LENTH		1024

#include "SServerObj.h"

class SLogger : public SSingleton <SLogger>, public SServerObj
{
private:
	friend class SSingleton<SLogger>;

private:
	//==============================================================
	// ���� ���� ������
	//--------------------------------------------------------------
	//�Էµ� ���� �ӽ��̸�
	char			m_chtmpFilepath[TMPSTRING_LENTH];
	//�Էµ� ���ϰ� ���� �̸�
	char			m_chFilename[50];
	//�Էµ� ���� ����
	FILE			*m_pFile;
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

	//���� ���� ���� ����, ����� TickTime����
	void Create( char* filename );
	//���� ���� ����
	BOOL OpenFile();

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

#endif

