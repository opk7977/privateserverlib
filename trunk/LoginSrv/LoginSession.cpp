#include "LoginSession.h"
#include "LoginProtocol.h"
#include "DataLeader.h"

#include "SPacket.h"
#include "SLogger.h"

#include "LogSrvMgr.h"

#include "DBSrvMgr.h"
#include "TmpSessionSpace.h"


SIMPLEMENT_DYNAMIC(LoginSession)
SIMPLEMENT_DYNCREATE(LoginSession)

LoginSession::LoginSession(void)
: isLogin(FALSE)
, m_isWaitting(FALSE)
{
	//singleTon��ü�� �̸� �޾� ���.
#ifdef _DEBUG
	m_logger	= &GetLogger;
#endif
	m_document	= &GetDocument;
#ifdef CONNECT_LOG_SERVER
	m_logSrv	= &GetLogSrvMgr;
#endif
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
#ifdef CONNECT_LOG_SERVER
	m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION, _T("LoginSession::OnCreate() IP:%s ����."), m_tIPAddr );
#endif
 
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
#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("%d���� ���ǵ��� ���� ��Ŷ id�Դϴ�.\n\n"), packet.GetID() );
#endif
#ifdef CONNECT_LOG_SERVER
		m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_WORRNING, _T("%d���� ���ǵ��� ���� ��Ŷ id�Դϴ�."), packet.GetID() );
#endif
		break;
	}
}

//==============================================================

void LoginSession::RecvLogSrvConnectOK()
{
	//log���� ������ ����
#ifdef CONNECT_LOG_SERVER
	m_logSrv->SetSession( this );
#endif

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
					_T("LoginSession::RecvLogSrvConnectOK()\n�α� ������ ����Ǿ����ϴ�.\n\n") );
#endif
}

void LoginSession::RecvLogSrvShotdown()
{
	//log���� ����
#ifdef CONNECT_LOG_SERVER
	m_logSrv->DisConnect();
#endif
	
#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
		_T("LoginSession::RecvLogSrvConnectOK()\n�α� �������� ������ ���� �Ǿ����ϴ�.\n\n") );
#endif
}
//--------------------------------------------------------------

void LoginSession::RecvDBConnectOK()
{
	//DB���� ���� ���� ������ ����
	m_dbSrvMgr->SetSession( this );

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
		_T("LoginSession::RecvDBConnectOK()\nDB ������ ����Ǿ����ϴ�.\n\n") );
#endif
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
#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("LoginSession::RecvDBCheckIDResult()\n%d�� index�� �ش��ϴ�session�� �����ϴ�.\n\n"), index );
#endif

#ifdef CONNECT_LOG_SERVER
		m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_WORRNING,
			_T("LoginSession::RecvDBCheckIDResult() %d�� index�� �ش��ϴ�session�� �����ϴ�."), index );
#endif
		return;
	}

	m_isWaitting = FALSE;

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
#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("LoginSession::RecvDBCreateAccountResult()\n%d�� index�� �ش��ϴ�session�� �����ϴ�.\n\n"), index );
#endif
#ifdef CONNECT_LOG_SERVER
		m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_WORRNING,
			_T("LoginSession::RecvDBCreateAccountResult() %d�� index�� �ش��ϴ�session�� �����ϴ�."), index );
#endif
		return;
	}

	m_isWaitting = FALSE;

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
#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("LoginSession::RecvDBCheckIDResult()\n%d�� index�� �ش��ϴ�session�� �����ϴ�.\n\n"), index );
#endif
#ifdef CONNECT_LOG_SERVER
		m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_WORRNING,
			_T("LoginSession::RecvDBCheckIDResult() %d�� index�� �ش��ϴ�session�� �����ϴ�."), index );
#endif
		return;
	}

	m_isWaitting = FALSE;

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

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
		_T("LoginSession::RecvCheckId()\n���̵� %s �ߺ� üũ��û�� ����.\n\n"), tstrID );
#endif
#ifdef CONNECT_LOG_SERVER
	m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
		_T("LoginSession::RecvCheckId() ���̵� %s �ߺ� üũ��û�� ����."), tstrID );
#endif

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

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
					_T("LoginSession::RecvCheckId()\n���̵� : %s\n�н����� : %s\n�̸��� : %s �������� ��û�� ����\n\n"),
					tstrID, tstrPW,	tstrMAIL );
#endif
#ifdef CONNECT_LOG_SERVER
	m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
					_T("LoginSession::RecvCheckId() ���̵� : %s\n�н����� : %s\n�̸��� : %s �������� ��û�� ����"),
					tstrID, tstrPW,	tstrMAIL );
#endif

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

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
		_T("LoginSession::RecvCheckId()\n���̵� %s ��� %s �α��� ��û�� ����.\n\n"), tstrID, tstrPW );
#endif
#ifdef CONNECT_LOG_SERVER
	m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
		_T("LoginSession::RecvCheckId() ���̵� %s ��� %s �α��� ��û�� ����."), tstrID, tstrPW );
#endif

	SendToDBTryLogin( IDSize, tstrID, PWSize, tstrPW );
}

//==============================================================

BOOL LoginSession::SendToDBCheckID( int size, TCHAR* uid )
{
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

	//ip�ּҿ� port��ȣ�� �־� �ش�.
	int size = strlen( m_document->LobbySrvIP );
	sendPacket << size;
	sendPacket.PutData( m_document->LobbySrvIP, size );
	sendPacket << m_document->LobbySrvPortNum;		//��Ʈ��ȣ

	if( SendPacket( sendPacket ) == sendPacket.GetPacketSize() )
		return TRUE;

	return FALSE;
}
