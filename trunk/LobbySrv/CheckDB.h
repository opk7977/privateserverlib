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
	void Release();

	//sessionId로 userID를 받아 간다
	void GetData( int in_sessionId, TCHAR* out_ID );

	//성공실패
	BOOL UpdateLogin( int sessionId, BOOL isLogin = TRUE );

	//음수:오류/ 0:로그인안되어 있음/ 1:로그인되어있음
	int IsLogin( int sessionID );
};

#define GetDB CheckDB::GetInstance()

