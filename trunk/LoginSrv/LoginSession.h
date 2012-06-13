#pragma once

#include "SSessionObj.h"

class SPacket;

class LoginDB;
class SLogger;
class DataLeader;
class LogSrvNet;
class LogMgr;
class DBSrvMgr;
class TmpSessionSpace;

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
	LoginDB*			m_dbMgr;		//db��ü
	SLogger*			m_logger;		//�α� ��ü
	DataLeader*			m_document;		//���� data
	LogSrvNet*			m_logSrv;		//�α� ����
	LogMgr*				m_logMgr;		//�α׸޴���
	DBSrvMgr*			m_dbSrvMgr;		//DB���� �޴���
	TmpSessionSpace*	m_tmpSpace;		//���⸦ �� session�� ���� ����
	//======================================
	
	//DB�α��� üũ�� ���� flag
	BOOL		isLogin;

	//DB�� ������ ��ٸ������� �˱� ���� flag
	BOOL		m_isWaitting;

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
	//LOG_SERVER_CONNECT_OK
	void RecvLogSrvConnectOK();

	//LOG_SERVER_SHOTDOWN
	void RecvLogSrvShotdown();

	//--------------------------------------
	// DB �������� Ŀ�´����̼�
	//--------------------------------------
	//DB_TO_OTHER_CONNECT_OK
	void RecvDBConnectOK();

	//DB_TO_LOGIN_CHECK_ID_RESULT
	void RecvDBCheckIDResult( SPacket& packet );

	//DB_TO_LOGIN_CREATE_ACCOUNT_RESULT
	void RecvDBCreateAccountResult( SPacket& packet );

	//DB_TO_LOGIN_LOGINRESULT
	void RecvDBLoginResult( SPacket& packet );

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

	//--------------------------------------
	// DB �������� Ŀ�´����̼�
	//--------------------------------------
	//LOGIN_TO_DB_CHECK_ID
	BOOL SendToDBCheckID( int size, TCHAR* uid );

	//LOGIN_TO_DB_CREATE_ACCOUNT
	BOOL SendToDBCreateAccount( int uidSize, TCHAR* uid, int pwSize, TCHAR* pw, int mailSize, TCHAR* mail );

	//LOGIN_TO_DB_TRYLOGIN
	BOOL SendToDBTryLogin( int uidSize, TCHAR* uid, int pwSize, TCHAR* pw );


	//--------------------------------------
	// client ���� Ŀ�´����̼�
	//--------------------------------------
	//SC_LOGIN_CHECK_ID_RESULT
	BOOL SendCheckIdResult( int index, BOOL result );

	//SC_LOGIN_CREATE_RESULT
	BOOL SendCreateResult( int index, BOOL result );

	//SC_LOGIN_LOGIN_RESULT
	BOOL SendLoginResult( int index, int result );
	BOOL SendLoginFailed( int result );
	BOOL SendLoginSuccess( int result );
};
