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
	void Relase();

	//sessionId�� userID�� �޾� ����
	void GetData( int in_sessionId, TCHAR* out_ID );
};

#define GetDB CheckDB::GetInstance()

