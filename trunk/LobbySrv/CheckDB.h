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
	//�ʱ�ȭ
	BOOL Init( TCHAR* filename );
	void Release();

	//sessionId�� userID�� �޾� ����
	void GetData( int in_sessionId, TCHAR* out_ID );

	//��������
	BOOL UpdateLogin( int sessionId, BOOL isLogin = TRUE );

	//����:����/ 0:�α��ξȵǾ� ����/ 1:�α��εǾ�����
	int IsLogin( int sessionID );
};

#define GetDB CheckDB::GetInstance()

