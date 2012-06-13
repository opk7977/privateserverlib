#pragma once

#include "SSessionObj.h"

class SPacket;

class LoginDB;
class SLogger;
class DataLeader;
class LogSrvNet;
class LogMgr;

//======================================
// �α��� ������ Multicommunication���ٴ�
// 1:1 communication�̱� ������
// ��ε��ɽ�Ʈ ���� �͵��� �ʿ� ����
//======================================

class LoginSession : public SSessionObj
{
public:
	SDECLARE_DYNAMIC(LoginSession)
	SDECLARE_DYNCREATE(LoginSession)

private:
	//======================================
	// Single Ton��ü
	//======================================
	LoginDB*	m_dbMgr;		//db��ü
	SLogger*	m_logger;		//�α� ��ü
	DataLeader*	m_document;		//���� data
	LogSrvNet*	m_logSrv;		//�α� ����
	LogMgr*		m_logMgr;		//�α׸޴���
	//======================================
	
	//DB�α��� üũ�� ���� flag
	BOOL		isLogin;

public:
	LoginSession(void);
	~LoginSession(void);

	void OnCreate();
	void OnDestroy();

	void PacketParsing( SPacket& packet );

	//======================================
	// �޾Ƽ� ó��
	//======================================
	//--------------------------------------
	// log �������� Ŀ�´����̼�
	//--------------------------------------
	// LOG_SERVER_CONNECT_OK
	void RecvLogSrvConnectOK();

	// LOG_SERVER_SHOTDOWN
	void RecvLogSrvShotdown();

	//--------------------------------------
	// client���� Ŀ�´����̼�
	//--------------------------------------
	//CS_LOGIN_CHECK_ID
	void RecvCheckId( SPacket& packet );

	//CS_LOGIN_CREATE_ACCOUNT
	void RecvCreateAccount( SPacket& packet );

	//CS_LOGIN_LOGIN
	void RecvTryLogin( SPacket& packet );


	//======================================
	// ������ ó��
	//======================================
	//SC_LOGIN_CONNECT_OK
	//�Լ� �ʿ� ����..

	//SC_LOGIN_CHECK_ID_RESULT
	BOOL SendCheckIdResult( int result );

	//SC_LOGIN_CREATE_RESULT
	BOOL SendCreateResult( int result );

	//SC_LOGIN_LOGIN_RESULT
	BOOL SendLoginFailed( int result );
	BOOL SendLoginSuccess( int result );
};
