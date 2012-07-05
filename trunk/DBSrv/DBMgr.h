#pragma once

// #include "SQuery.h"
#include "DataBaseObj.h"

class SQuery;
class RankDataMgr;
class SLogger;
class PlayerObj;

class DBMgr : public DataBaseObj, public SSingleton <DBMgr>
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

	SQuery*			m_query;

private:
	DBMgr(void);
	~DBMgr(void);

public:	
	// DB연결 및 초기화
	//sql db와 연결
	virtual BOOL Init( TCHAR* dbname, TCHAR* _id, TCHAR* _pw );
	virtual void Release();

	//--------------------------------------------------------------
	// DB에서 랭크 정보 받아와서 셋팅해 주기
	//--------------------------------------------------------------
	virtual BOOL RankDataSetting();
	//--------------------------------------------------------------

	//id체크
	virtual BOOL CheckID( TCHAR* _id );
	//계정 생성
	virtual BOOL CreateAccount( TCHAR* _id, TCHAR* _pw, TCHAR* email );
	//로그인
	virtual int TryLogin( TCHAR* _id, TCHAR* _pw );
	//UserData를 받아 셋팅
	virtual BOOL SettingUserData( int sessionID, PlayerObj* player );
	//DB를 업데이트
	virtual BOOL UpdateUserData( int sessionID, int rankId, int rankPoint, int accumulKill, int accumulDeath );
};

#define GetDBMgr DBMgr::GetInstance()

