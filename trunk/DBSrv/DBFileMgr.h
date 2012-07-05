#pragma once

// #include "SQuery.h"
#include "DataBaseObj.h"

class SQuery;
class RankDataMgr;
class SLogger;
class PlayerObj;

class DBFileMgr : public DataBaseObj, public SSingleton <DBFileMgr>
{
private:
	friend class SSingleton<DBFileMgr>;

private:
	//======================================
	// db �̱���
	//======================================
	RankDataMgr*	m_rankMgr;
	SLogger*		m_logger;
	//======================================

	SQuery*			m_query;

private:
	DBFileMgr(void);
	~DBFileMgr(void);

public:
	// DB���� �� �ʱ�ȭ
	//mdb ���ϰ� ����
	virtual BOOL Init( TCHAR* dbname, TCHAR* _id, TCHAR* _pw );
	virtual void Release();

	//--------------------------------------------------------------
	// DB���� ��ũ ���� �޾ƿͼ� ������ �ֱ�
	//--------------------------------------------------------------
	virtual BOOL RankDataSetting();
	//--------------------------------------------------------------

	//idüũ
	virtual BOOL CheckID( TCHAR* _id );
	//���� ����
	virtual BOOL CreateAccount( TCHAR* _id, TCHAR* _pw, TCHAR* email );
	//�α���
	virtual int TryLogin( TCHAR* _id, TCHAR* _pw );
	//UserData�� �޾� ����
	virtual BOOL SettingUserData( int sessionID, PlayerObj* player );
	//DB�� ������Ʈ
	virtual BOOL UpdateUserData( int sessionID, int rankId, int rankPoint, int accumulKill, int accumulDeath );
};

#define GetDBFileMgr DBFileMgr::GetInstance()

