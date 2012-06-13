#pragma once

#include "SQuery.h"

class SSqlConn : public SServerObj
{
private:
	SQuery	m_query;
	
public:
	SSqlConn(void);
	~SSqlConn(void);

	//DB에 연결( 서버로 연결 )
	BOOL Init( TCHAR* srvName, TCHAR* _id, TCHAR* _pw );
	void Release();

	BOOL PrintAllRanksInfo();
	int CheckID( TCHAR* _id );

	int CreateAccount( TCHAR* _id, TCHAR* _pw, TCHAR* _mail );
};
