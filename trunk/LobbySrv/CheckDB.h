#pragma once

#include "SQuery.h"

class CheckDB : public SSingleton <CheckDB>
{
private:
	friend class SSingleton<CheckDB>;

private:
	SQuery		m_query;

private:
	CheckDB(void);
	~CheckDB(void);

public:
	//초기화
	BOOL Init( TCHAR* filename );
	void Relase();

	//sessionId로 userID를 받아 간다
	void GetData( int in_sessionId, TCHAR* out_ID );
};

#define GetDB CheckDB::GetInstance()

