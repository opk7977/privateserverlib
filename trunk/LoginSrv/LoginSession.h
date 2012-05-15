#pragma once

#include "SSessionObj.h"

class SPacket;

class LoginDB;
class SLogger;
class DataLeader;

//======================================
// 로그인 세션은 MultiConnect보다는
// 1:1 Connect이기 때문에
// 브로드케스트 같은 것들은 필요 없음
//======================================

class LoginSession : public SSessionObj
{
public:
	SDECLARE_DYNAMIC(LoginSession)
	SDECLARE_DYNCREATE(LoginSession)

private:
	//======================================
	// Single Ton객체
	//======================================
	LoginDB*	m_dbMgr;		//db객체
	SLogger*	m_logger;		//로그 객체
	DataLeader*	m_document;		//서버 data
	//======================================
	
	//DB로그인 체크를 위한 flag
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
	BOOL SendLoginFailed( int result );
	BOOL SendLoginSuccess( int result );

	//BOOL SendLoginResult( int result );
};
