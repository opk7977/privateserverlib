#pragma once

#include "SQuery.h"

#pragma comment( lib, "odbc32.lib" )

class LogDataBase : public SSingleton <LogDataBase>
{
private:
	friend class SSingleton<LogDataBase>;

private:
	SQuery		m_query;

private:
	LogDataBase(void);
	~LogDataBase(void);

public:
	//���� �̸��� Ȯ���� ����!
	BOOL Init( TCHAR* filename );
	void Release();

	//======================================
	// DB�Լ���
	//======================================
	//--------------------------------------
	// �ӽ��Լ�
	//--------------------------------------
	BOOL CreateServerTbl();
	BOOL CreateTableListTbl();
	//BOOL CreateLogTable();
};

#define GetDB LogDataBase::GetInstance()

