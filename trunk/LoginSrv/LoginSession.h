#pragma once

#include "SSessionObj.h"
#include "SPacket.h"

class LoginDB;
class SLogger;

class LoginSession : public SSessionObj
{
public:
	SDECLARE_DYNAMIC(LoginSession)
	SDECLARE_DYNCREATE(LoginSession)

private:
	//db객체
	LoginDB*	m_dbMgr;
	//로그 객체
	SLogger*	m_logger;

	BOOL		isLogin;

public:
	LoginSession(void);
	~LoginSession(void);

	void OnCreate();
	void OnDestroy();

	void PacketParsing( SPacket& packet );


	//======================================
	// 받아서 처리
	//======================================

	//CS_LOGIN_CHECK_ID
	void RecvCheckId( SPacket& packet );

	//CS_LOGIN_CREATE_ACCOUNT
	void RecvCreateAccount( SPacket& packet );

	//CS_LOGIN_LOGIN
	void RecvTryLogin( SPacket& packet );


	//======================================
	// 보내기 처리
	//======================================

	//SC_LOGIN_CONNECT_OK
	//함수 필요 없음..

	//SC_LOGIN_CHECK_ID_RESULT
	BOOL SendCheckIdResult( int result );

	//SC_LOGIN_CREATE_RESULT
	BOOL SendCreateResult( int result );

	//SC_LOGIN_LOGIN_RESULT
	BOOL SendLoginResult( int result );
};
