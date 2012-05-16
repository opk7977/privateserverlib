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
	//파일 이름에 확장자 포함!
	BOOL Init( TCHAR* filename );
	void Release();

	//======================================
	// DB함수들
	//======================================
	//--------------------------------------
	// 임시함수
	//--------------------------------------
	BOOL CreateServerTbl();
	BOOL CreateTableListTbl();
	//BOOL CreateLogTable();
};

#define GetDB LogDataBase::GetInstance()

