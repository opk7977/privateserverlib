#include "LoginSession.h"
#include "LoginProtocol.h"
#include "LoginDB.h"
#include "DataLeader.h"

#include "SPacket.h"
#include "SLogger.h"

#include "LogSrvNet.h"
#include "LogMgr.h"


SIMPLEMENT_DYNAMIC(LoginSession)
SIMPLEMENT_DYNCREATE(LoginSession)

LoginSession::LoginSession(void)
: isLogin(FALSE)
{
	//singleTon��ü�� �̸� �޾� ���.
	m_dbMgr		= &GetDBMgr;
	m_logger	= &GetLogger;
	m_document	= &GetDocument;
	m_logSrv	= &GetSrvNet;
	m_logMgr	= &GetLog;
}

LoginSession::~LoginSession(void)
{
}

void LoginSession::OnCreate()
{
	SSession::OnCreate();

	//�α׼����� ����
// 	m_logMgr->SendLog( 8800, LogMgr::LOG_LEVEL_INFORMATION, _T("[LoginSession::RecvLogSrvConnectOK()]_1. ID ������ ��Ȯ�� �� �ǳ�?.\n") );

	//���ῡ �����ߴٴ� packet�� ������.
	SendPacket( SC_LOGIN_CONNECT_OK );
}

void LoginSession::OnDestroy()
{
	//�׳� ���� �ָ� ��
	SSession::OnDestroy();
}

//--------------------------------------------------------------

void LoginSession::PacketParsing( SPacket& packet )
{
	switch( packet.GetID() )
	{
	//======================================> LogServer
	case LOG_SERVER_CONNECT_OK:
		RecvLogSrvConnectOK();
		break;
	case LOG_SERVER_SHOTDOWN:
		RecvLogSrvShotdown();
		break;
	//======================================> Client
	case CS_LOGIN_CHECK_ID:
		RecvCheckId( packet );
		break;
	case CS_LOGIN_CREATE_ACCOUNT:
		RecvCreateAccount( packet );
		break;
	case CS_LOGIN_LOGIN:
		RecvTryLogin( packet );
		break;
	default:
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("���ǵ��� ���� ��Ŷ id�Դϴ�.\n\n") );
	}
}

//==============================================================

void LoginSession::RecvLogSrvConnectOK()
{
	SSynchronize Sync( this );

	//log���� ������ ����
	m_logSrv->SetSession( this );

	m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
					_T("LoginSession::RecvLogSrvConnectOK()\n�α� ������ ����Ǿ����ϴ�.\n\n") );
}

void LoginSession::RecvLogSrvShotdown()
{
	//�α׼��� ������ Ǯ�� ����
	SSynchronize Sync( this );

	//log���� ������ ����
	m_logSrv->DisConnect();

	m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
					_T("LoginSession::RecvLogSrvConnectOK()\n�α� �������� ������ ���� �Ǿ����ϴ�.\n\n") );
}

//--------------------------------------------------------------

//CS_LOGIN_CHECK_ID
void LoginSession::RecvCheckId( SPacket& packet )
{
	//======================================
	// ID�ߺ� üũ
	//======================================
	TCHAR tstrID[64] = {0,};
	int idSize;

	SSynchronize Sync( this );

	//--------------------------------------
	// ������ ����
	//--------------------------------------
	packet >> idSize;
	packet.GetData( tstrID, idSize );
	//--------------------------------------

	//ID�ߺ� üũ
	int result = m_dbMgr->CheckId( tstrID );

	SendCheckIdResult( result );
}

//CS_LOGIN_CREATE_ACCOUNT
void LoginSession::RecvCreateAccount( SPacket& packet )
{
	//======================================
	// ��������
	//======================================
	TCHAR tstrID[64] = {0,};
	TCHAR tstrPW[64] = {0,};
	TCHAR tstrMAIL[128] = {0,};
	int tstrSize;

	SSynchronize Sync( this );

	//--------------------------------------
	// ������ ����
	//--------------------------------------
	packet >> tstrSize;
	packet.GetData( tstrID, tstrSize );
	packet >> tstrSize;
	packet.GetData( tstrPW, tstrSize );
	packet >> tstrSize;
	packet.GetData( tstrMAIL, tstrSize );
	//--------------------------------------

	m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
					_T("LoginSession::RecvCheckId()\n���̵� : %s\n�н����� : %s\n�̸��� : %s\n"),
					tstrID, tstrPW,	tstrMAIL );

	//���� ����
	int result = m_dbMgr->AddAccount( tstrID, tstrPW, tstrMAIL );	

	SendCreateResult( result );
}

//CS_LOGIN_LOGIN
void LoginSession::RecvTryLogin( SPacket& packet )
{
	//======================================
	// �α���
	//======================================
	TCHAR tstrID[64];
	TCHAR tstrPW[64];
	int tstrSize;

	SSynchronize Sync( this );

	//--------------------------------------
	// ������ ����
	//--------------------------------------
	packet >> tstrSize;
	packet.GetData( tstrID, tstrSize );
	packet >> tstrSize;
	packet.GetData( tstrPW, tstrSize );
	//--------------------------------------

	//�α���!
	int sessionId = m_dbMgr->TryLogin( tstrID, tstrPW );

	if( sessionId > 0 )
	{
		//�α����� üũ������ �ߺ����� Ȯ���Ѵ�
		int retval = m_dbMgr->IsLogin( sessionId );

		if( retval < 0 )
		{
			//����
			sessionId = retval;
		}
		else if( retval > 0 )
		{
			//�̹� �α��εǾ� ����
			m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("LoginSession::RecvTryLogin()\nIsLogin�̹� �α��� �Ǿ� �ִ� ID�Դϴ�.\n\n") );
			sessionId = PRE_LOGIN_ID;
		}
		// retval == 0�̸� �α���
	}


	if( sessionId > 0 )
		SendLoginSuccess( sessionId );
	else
		SendLoginFailed( sessionId );
}

//==============================================================

//SC_LOGIN_CHECK_ID_RESULT
BOOL LoginSession::SendCheckIdResult( int result )
{
	SPacket sendPacket;

	sendPacket.SetID( SC_LOGIN_CHECK_ID_RESULT );
	sendPacket << result;

	if( SendPacket( sendPacket ) == sendPacket.GetPacketSize() )
		return TRUE;

	return FALSE;
}

//SC_LOGIN_CREATE_RESULT
BOOL LoginSession::SendCreateResult( int result )
{
	SPacket sendPacket;

	sendPacket.SetID( SC_LOGIN_CREATE_RESULT );
	sendPacket << result;

	if( SendPacket( sendPacket ) == sendPacket.GetPacketSize() )
		return TRUE;

	return FALSE;
}

BOOL LoginSession::SendLoginFailed( int result )
{
	//--------------------------------------
	// �α��� ����!
	//--------------------------------------
	SPacket sendPacket;

	sendPacket.SetID( SC_LOGIN_LOGIN_RESULT );
	sendPacket << result;

	if( SendPacket( sendPacket ) == sendPacket.GetPacketSize() )
		return TRUE;

	return FALSE;
}

BOOL LoginSession::SendLoginSuccess( int result )
{
	//--------------------------------------
	// �α��� ����!!
	//--------------------------------------
	SPacket sendPacket;

	sendPacket.SetID( SC_LOGIN_LOGIN_RESULT );
	sendPacket << result;

	//�α��� ó��
	//DB�� ǥ��
	m_dbMgr->UpdateLogin( result );

	//ip�ּҿ� port��ȣ�� �־� �ش�.
	int size = strlen( m_document->LobbySrvIP );
	sendPacket << size;
	sendPacket.PutData( m_document->LobbySrvIP, size );
	sendPacket << m_document->LobbySrvPortNum;		//��Ʈ��ȣ

	if( SendPacket( sendPacket ) == sendPacket.GetPacketSize() )
		return TRUE;

	return FALSE;
}
