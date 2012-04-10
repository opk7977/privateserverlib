#pragma once

#include "SQuery.h"

class LoginDB : public SSingleton <LoginDB>
{
private:
	friend class SSingleton<LoginDB>;

private:
	SQuery m_query;

private:
	LoginDB(void);
	~LoginDB(void);

public:
	//초기화
	BOOL Init( TCHAR* filename );
	void Relase();

	//id확인
	// return 값 :
	// -1 : id가 존재합니다.
	//  1 : 사용할 수 있는 id
	int CheckId( TCHAR* tstrID );

	//계정 추가
	// return 값 :
	// -1 : 추가 실패
	//  1 : 추가 성공
	int AddAccount( TCHAR* tstrID, TCHAR* tstrPW, TCHAR* tstrMAILE );

	//로그인 결과를 받아 온다
	// return 값 :
	// -1 : id가 존재 하지 않습니다.
	//  0 : pw가 다릅니다
	//  n : 쿼리 성공으로 나온 CharacterIndex ID
	int TryLogin( TCHAR* tstrID, TCHAR* tstrPW );
};

#define GetDBMgr LoginDB::GetInstance()

