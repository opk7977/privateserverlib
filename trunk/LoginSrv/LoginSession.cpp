#include "LoginSession.h"
#include "LoginProtocol.h"
#include "DataLeader.h"

#include "SPacket.h"
#include "SLogger.h"

#include "LogSrvNet.h"
#include "LogMgr.h"

#include "DBSrvMgr.h"
#include "TmpSessionSpace.h"


SIMPLEMENT_DYNAMIC(LoginSession)
SIMPLEMENT_DYNCREATE(LoginSession)

LoginSession::LoginSession(void)
: isLogin(FALSE)
, m_isWaitting(FALSE)
{
	//singleTon��ü�� �̸� �޾� ���.
	m_logger	= &GetLogger;
	m_document	= &GetDocument;
	m_logSrv	= &GetSrvNet;
	m_logMgr	= &GetLog;
	m_dbSrvMgr	= &GetDBSrv;
	m_tmpSpace	= &GetTmpSpace;
}

LoginSession::~LoginSession(void)
{
}

void LoginSession::OnCreate()
{
	SSynchronize Sync( this );

	SSession::OnCreate();

	//�α׼����� ����
// 	m_logMgr->SendLog( 8800, LogMgr::LOG_LEVEL_INFORMATION, _T("[LoginSession::RecvLogSrvConnectOK()]_1. ID ������ ��Ȯ�� �� �ǳ�?.\n") );

	//���ῡ �����ߴٴ� packet�� ������.
	SendPacket( SC_LOGIN_CONNECT_OK );
}

void LoginSession::OnDestroy()
{
	SSynchronize Sync( this );

	//�׳� ���� �ָ� ��
	SSession::OnDestroy();
}

//--------------------------------------------------------------

void LoginSession::PacketParsing( SPacket& packet )
{
	SSynchronize Sync( this );

	switch( packet.GetID() )
	{
	//======================================> LogServer
	case LOG_SERVER_CONNECT_OK:
		RecvLogSrvConnectOK();
		break;
	case LOG_SERVER_SHOTDOWN:
		RecvLogSrvShotdown();
		break;
	//======================================> DBServer
	case DB_TO_OTHER_CONNECT_OK:
		RecvDBConnectOK();
		break;
	case DB_TO_LOGIN_CHECK_ID_RESULT:
		RecvDBCheckIDResult( packet );
		break;
	case DB_TO_LOGIN_CREATE_ACCOUNT_RESULT:
		RecvDBCreateAccountResult( packet );
		break;
	case DB_TO_LOGIN_LOGINRESULT:
		RecvDBLoginResult( packet );
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
	//SSynchronize Sync( this );

	//log���� ������ ����
	m_logSrv->SetSession( this );

	m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
					_T("LoginSession::RecvLogSrvConnectOK()\n�α� ������ ����Ǿ����ϴ�.\n\n") );
}

void LoginSession::RecvLogSrvShotdown()
{
	//�α׼��� ������ Ǯ�� ����
	//SSynchronize Sync( this );

	//log���� ������ ����
	m_logSrv->DisConnect();

	m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
					_T("LoginSession::RecvLogSrvConnectOK()\n�α� �������� ������ ���� �Ǿ����ϴ�.\n\n") );
}
//--------------------------------------------------------------

void LoginSession::RecvDBConnectOK()
{
	//DB���� ���� ���� ������ ����
	m_dbSrvMgr->SetSession( this );

	m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
		_T("LoginSession::RecvDBConnectOK()\nDB ������ ����Ǿ����ϴ�.\n\n") );
}

void LoginSession::RecvDBCheckIDResult( SPacket& packet )
{
	//���̵� �ߺ� �˻� ��� ��Ŷ�� ����
	int index;
	BOOL result;

	packet >> index;
	packet >> result;

	//������� session������ �޾� �´�
	LoginSession* tmpSession = m_tmpSpace->FindSessionByIndex( index );
	if( tmpSession == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("LoginSession::RecvDBCheckIDResult()\nindex�� �ش��ϴ�session�� �����ϴ�.\n\n") );
		return;
	}

	tmpSession->SendCheckIdResult( index, result );
}

void LoginSession::RecvDBCreateAccountResult( SPacket& packet )
{
	int index;
	BOOL result;

	packet >> index;
	packet >> result;

	//������� session������ �޾� �´�
	LoginSession* tmpSession = m_tmpSpace->FindSessionByIndex( index );
	if( tmpSession == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("LoginSession::RecvDBCheckIDResult()\nindex�� �ش��ϴ�session�� �����ϴ�.\n\n") );
		return;
	}

	tmpSession->SendCreateResult( index, result );
}

void LoginSession::RecvDBLoginResult( SPacket& packet )
{
	int index, result;

	packet >> index;
	packet >> result;

	//������� session������ �޾� �´�
	LoginSession* tmpSession = m_tmpSpace->FindSessionByIndex( index );
	if( tmpSession == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("LoginSession::RecvDBCheckIDResult()\nindex�� �ش��ϴ�session�� �����ϴ�.\n\n") );
		return;
	}

	tmpSession->SendLoginResult( index, result );
}

//--------------------------------------------------------------

//CS_LOGIN_CHECK_ID
void LoginSession::RecvCheckId( SPacket& packet )
{
	//�̹� DB�� ������ ��ٸ��� �ִ� ���̹Ƿ� �ƹ��͵� ���� �ʴ´�
	if( m_isWaitting )
		return;

	m_isWaitting = TRUE;

	//======================================
	// ID�ߺ� üũ
	//======================================
	TCHAR tstrID[64] = {0,};
	int idSize;

	//--------------------------------------
	// ������ ����
	//--------------------------------------
	packet >> idSize;
	packet.GetData( tstrID, idSize );
	//--------------------------------------

	SendToDBCheckID( idSize, tstrID );
}

//CS_LOGIN_CREATE_ACCOUNT
void LoginSession::RecvCreateAccount( SPacket& packet )
{
	//�̹� DB�� ������ ��ٸ��� �ִ� ���̹Ƿ� �ƹ��͵� ���� �ʴ´�
	if( m_isWaitting )
		return;

	m_isWaitting = TRUE;

	//======================================
	// ��������
	//======================================
	TCHAR tstrID[64] = {0,};
	TCHAR tstrPW[64] = {0,};
	TCHAR tstrMAIL[128] = {0,};
	int IDSize, PWSize, MAILSize;

	//--------------------------------------
	// ������ ����
	//--------------------------------------
	packet >> IDSize;
	packet.GetData( tstrID, IDSize );
	packet >> PWSize;
	packet.GetData( tstrPW, PWSize );
	packet >> MAILSize;
	packet.GetData( tstrMAIL, MAILSize );
	//--------------------------------------

	m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
					_T("LoginSession::RecvCheckId()\n���̵� : %s\n�н����� : %s\n�̸��� : %s\n"),
					tstrID, tstrPW,	tstrMAIL );

	SendToDBCreateAccount( IDSize, tstrID, PWSize, tstrPW, MAILSize, tstrMAIL );
}

//CS_LOGIN_LOGIN
void LoginSession::RecvTryLogin( SPacket& packet )
{
	//�̹� DB�� ������ ��ٸ��� �ִ� ���̹Ƿ� �ƹ��͵� ���� �ʴ´�
	if( m_isWaitting )
		return;

	m_isWaitting = TRUE;

	//======================================
	// �α���
	//======================================
	TCHAR tstrID[64];
	TCHAR tstrPW[64];
	int IDSize, PWSize;

	//--------------------------------------
	// ������ ����
	//--------------------------------------
	packet >> IDSize;
	packet.GetData( tstrID, IDSize );
	packet >> PWSize;
	packet.GetData( tstrPW, PWSize );
	//--------------------------------------

	SendToDBTryLogin( IDSize, tstrID, PWSize, tstrPW );

// 	//�α���!
// 	int sessionId = m_dbMgr->TryLogin( tstrID, tstrPW );
// 
// 	if( sessionId > 0 )
// 	{
// 		//�α����� üũ������ �ߺ����� Ȯ���Ѵ�
// 		int retval = m_dbMgr->IsLogin( sessionId );
// 
// 		if( retval < 0 )
// 		{
// 			//����
// 			sessionId = retval;
// 		}
// 		else if( retval > 0 )
// 		{
// 			//�̹� �α��εǾ� ����
// 			m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("LoginSession::RecvTryLogin()\nIsLogin�̹� �α��� �Ǿ� �ִ� ID�Դϴ�.\n\n") );
// 			sessionId = PRE_LOGIN_ID;
// 		}
// 		// retval == 0�̸� �α���
// 	}
// 
// 
// 	if( sessionId > 0 )
// 		SendLoginSuccess( sessionId );
// 	else
// 		SendLoginFailed( sessionId );
}

//==============================================================

BOOL LoginSession::SendToDBCheckID( int size, TCHAR* uid )
{
	//�̹� DB�� ������ ��ٸ��� �ִ� ���̹Ƿ� �ƹ��͵� ���� �ʴ´�
// 	if( m_isWaitting )
// 		return;

	//index�� �ϳ� �޾� �´�
	int index = m_tmpSpace->GetIndex();
	//�켱 ���⿭�� ���� �ְ�
	m_tmpSpace->WaitForDBSignal( index, this );

	//��Ŷ �����
	SPacket sendPacket( LOGIN_TO_DB_CHECK_ID );
	sendPacket << index;
	sendPacket << size;
	sendPacket.PutData( uid, size );

	//db�� ����
	m_dbSrvMgr->SendToDBServer( sendPacket );

	return TRUE;
}

BOOL LoginSession::SendToDBCreateAccount( int uidSize, TCHAR* uid, int pwSize, TCHAR* pw, int mailSize, TCHAR* mail )
{
	//index�� �ϳ� �޾� �´�
	int index = m_tmpSpace->GetIndex();
	//�켱 ���⿭�� ���� �ְ�
	m_tmpSpace->WaitForDBSignal( index, this );

	SPacket sendPacket( LOGIN_TO_DB_CREATE_ACCOUNT );
	sendPacket << index;
	sendPacket << uidSize;
	sendPacket.PutData( uid, uidSize );
	sendPacket << pwSize;
	sendPacket.PutData( pw, pwSize );
	sendPacket << mailSize;
	sendPacket.PutData( mail, mailSize );

	//db�� ����
	m_dbSrvMgr->SendToDBServer( sendPacket );
	
	return TRUE;
}

BOOL LoginSession::SendToDBTryLogin( int uidSize, TCHAR* uid, int pwSize, TCHAR* pw )
{
	//index�� �ϳ� �޾� �´�
	int index = m_tmpSpace->GetIndex();
	//�켱 ���⿭�� ���� �ְ�
	m_tmpSpace->WaitForDBSignal( index, this );

	SPacket sendPacket( LOGIN_TO_DB_TRYLOGIN );
	sendPacket << index;
	sendPacket << uidSize;
	sendPacket.PutData( uid, uidSize );
	sendPacket << pwSize;
	sendPacket.PutData( pw, pwSize );

	//db�� ����
	m_dbSrvMgr->SendToDBServer( sendPacket );

	return TRUE;
}

//--------------------------------------------------------------

//SC_LOGIN_CHECK_ID_RESULT
BOOL LoginSession::SendCheckIdResult( int index, BOOL result )
{
	SPacket sendPacket;

	sendPacket.SetID( SC_LOGIN_CHECK_ID_RESULT );
	sendPacket << result;

	BOOL sendResult = FALSE;
	if( SendPacket( sendPacket ) == sendPacket.GetPacketSize() )
		sendResult = TRUE;

	//��� �������� ������
	m_tmpSpace->GetSignal( index );

	//�ٽ� FALSE�� �ٲ� �ش�
	m_isWaitting = FALSE;

	return sendResult;
}

//SC_LOGIN_CREATE_RESULT
BOOL LoginSession::SendCreateResult( int index, BOOL result )
{
	SPacket sendPacket;

	sendPacket.SetID( SC_LOGIN_CREATE_RESULT );
	sendPacket << result;

	BOOL sendResult = FALSE;
	if( SendPacket( sendPacket ) == sendPacket.GetPacketSize() )
		sendResult = TRUE;

	//��� �������� ������
	m_tmpSpace->GetSignal( index );

	//�ٽ� FALSE�� �ٲ� �ش�
	m_isWaitting = FALSE;

	return sendResult;
}

BOOL LoginSession::SendLoginResult( int index, int result )
{
	BOOL SendResult;

	if( result <= 0 )
		SendResult = SendLoginFailed( result );
	else
		SendResult = SendLoginSuccess( result );

	//��� �������� ������
	m_tmpSpace->GetSignal( index );

	//�ٽ� FALSE�� �ٲ� �ش�
	m_isWaitting = FALSE;

	return SendResult;
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
	//m_dbMgr->UpdateLogin( result );

	//ip�ּҿ� port��ȣ�� �־� �ش�.
	int size = strlen( m_document->LobbySrvIP );
	sendPacket << size;
	sendPacket.PutData( m_document->LobbySrvIP, size );
	sendPacket << m_document->LobbySrvPortNum;		//��Ʈ��ȣ

	if( SendPacket( sendPacket ) == sendPacket.GetPacketSize() )
		return TRUE;

	return FALSE;
}
