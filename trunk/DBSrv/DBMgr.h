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
	// db �̱���
	//======================================
	RankDataMgr*	m_rankMgr;
	SLogger*		m_logger;
	//======================================

	SQuery			m_query;

public:
	DBMgr(void);
	~DBMgr(void);
	
	// DB���� �� �ʱ�ȭ
	BOOL Init( TCHAR* dbname, TCHAR* _id, TCHAR* _pw );
	void Release();

	//--------------------------------------------------------------
	// DB���� ��ũ ���� �޾ƿͼ� ������ �ֱ�
	//--------------------------------------------------------------
	BOOL RankDataSetting();
	//--------------------------------------------------------------

	//idüũ
	BOOL CheckID( TCHAR* _id );
	//���� ����
	BOOL CreateAccount( TCHAR* _id, TCHAR* _pw, TCHAR* email );
	//�α���
	int TryLogin( TCHAR* _id, TCHAR* _pw );
	//UserData�� �޾� ����
	BOOL SettingUserData( int sessionID, PlayerObj* player );
	//DB�� ������Ʈ
	BOOL UpdateUserData( int sessionID, int rankId, int rankPoint, int accumulKill, int accumulDeath );
};

#define GetDBMgr DBMgr::GetInstance()

