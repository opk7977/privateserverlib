#pragma once

#include "SQuery.h"

class SLogger;
class LogMgr;
class DataLeader;

class LoginDB : public SSingleton <LoginDB>
{
private:
	friend class SSingleton<LoginDB>;

private:
	SQuery		m_query;

	//======================================
	// singleTon ��ü
	//======================================
	SLogger*	m_logger;
	LogMgr*		m_logSrv;
	DataLeader*	m_document;
	

private:
	LoginDB(void);
	~LoginDB(void);

public:
	//�ʱ�ȭ
	BOOL Init( TCHAR* filename );
	void Release();

	//idȮ��
	// return �� :
	// -1 : id�� �����մϴ�.
	//  1 : ����� �� �ִ� id
	int CheckId( TCHAR* tstrID );

	//���� �߰�
	// return �� :
	// -1 : �߰� ����
	//  1 : �߰� ����
	int AddAccount( TCHAR* tstrID, TCHAR* tstrPW, TCHAR* tstrMAILE );

	//�α��� ����� �޾� �´�
	// return �� :
	// -1 : id�� ���� ���� �ʽ��ϴ�.
	//  0 : pw�� �ٸ��ϴ�
	//  n : ���� �������� ���� CharacterIndex ID
	int TryLogin( TCHAR* tstrID, TCHAR* tstrPW );

	//������ ���¸� �α��λ��� Ȥ�� �α׾ƿ� ���·� �ٲ۴�
	BOOL UpdateLogin( int sessionId, BOOL isLogin = TRUE );

	//������ �α��λ��¸� �޾� �´�
	int IsLogin( int sessionID );
};

#define GetDBMgr LoginDB::GetInstance()

