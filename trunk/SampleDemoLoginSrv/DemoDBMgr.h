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

	//�ʱ�ȭ
	BOOL Init( TCHAR* filename );
	void Release();

	//�α��� ����� �޾� �´�
	// return �� :
	// -1 : id�� ���� ���� �ʽ��ϴ�.
	//  0 : pw�� �ٸ��ϴ�
	//  n : ���� �������� ���� CharacterIndex ID
	int AskLogin( TCHAR* charID, TCHAR* charPW );
};

#define GetDBMgr DemoDBMgr::GetInstance()
