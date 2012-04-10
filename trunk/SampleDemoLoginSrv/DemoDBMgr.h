#pragma once

#include "SServerStd.h"
#include "SQuery.h"

class DemoDBMgr : public SSingleton <DemoDBMgr>
{
private:
	friend class SSingleton<DemoDBMgr>;

private:
	SQuery m_query;

public:
	DemoDBMgr(void);
	~DemoDBMgr(void);

	//초기화
	BOOL Init( TCHAR* filename );
	void Release();

	//로그인 결과를 받아 온다
	// return 값 :
	// -1 : id가 존재 하지 않습니다.
	//  0 : pw가 다릅니다
	//  n : 쿼리 성공으로 나온 CharacterIndex ID
	int AskLogin( TCHAR* charID, TCHAR* charPW );
};

#define GetDBMgr DemoDBMgr::GetInstance()
