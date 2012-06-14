#pragma once

#include "SQuery.h"

class RankDataMgr;
class SLogger;
class PlayerObj;

class DBMgr : public SSingleton <DBMgr>
{
private:
	friend class SSingleton<DBMgr>;

private:
	//======================================
	// db 싱글턴
	//======================================
	RankDataMgr*	m_rankMgr;
	SLogger*		m_logger;
	//======================================

	SQuery			m_query;

public:
	DBMgr(void);
	~DBMgr(void);
	
	// DB연결 및 초기화
	BOOL Init( TCHAR* dbname, TCHAR* _id, TCHAR* _pw );
	void Release();

	//--------------------------------------------------------------
	// DB에서 랭크 정보 받아와서 셋팅해 주기
	//--------------------------------------------------------------
	BOOL RankDataSetting();
	//--------------------------------------------------------------

	//id체크
	BOOL CheckID( TCHAR* _id );
	//계정 생성
	BOOL CreateAccount( TCHAR* _id, TCHAR* _pw, TCHAR* email );
	//로그인
	int TryLogin( TCHAR* _id, TCHAR* _pw );
	//UserData를 받아 셋팅
	BOOL SettingUserData( int sessionID, PlayerObj* player );
	//DB를 업데이트
	BOOL UpdateUserData( int sessionID, int rankId, int rankPoint, int accumulKill, int accumulDeath );
};

#define GetDBMgr DBMgr::GetInstance()

