#pragma once

#include "SServerStd.h"

class SrvCtrl : public SSingleton <SrvCtrl>
{
private:
	friend class SSingleton<SrvCtrl>;

private:
	BOOL		m_isStartServer;

	char		m_Dir[512];

public:
	SrvCtrl(void);
	~SrvCtrl(void);

	//��������/���ῡ �ʿ��� ��� ���� ã��
	BOOL CheckFindNeedAllFile();

	//start.bat�����ϱ�
	BOOL StartServer( char* srvName );

	//end.bat�����ϱ�
	void EndServer();
};

#define GetSrvCtrl SrvCtrl::GetInstance()

