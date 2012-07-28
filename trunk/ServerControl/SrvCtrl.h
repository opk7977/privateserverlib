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

	//서버실행/종료에 필요한 모든 파일 찾기
	BOOL CheckFindNeedAllFile();

	//start.bat실행하기
	BOOL StartServer( char* srvName );

	//end.bat실행하기
	void EndServer();
};

#define GetSrvCtrl SrvCtrl::GetInstance()

