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
	//�ʱ�ȭ
	BOOL Init( TCHAR* filename );
	void Relase();

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
};

#define GetDBMgr LoginDB::GetInstance()

