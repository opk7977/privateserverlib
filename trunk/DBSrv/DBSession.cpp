#include "DBSession.h"
#include "DBProtocol.h"

#include "SrvMgr.h"
#include "DBMgr.h"
#include "PlayerMgr.h"

#include "SLogger.h"
#include "LogSrvMgr.h"

SIMPLEMENT_DYNAMIC(DBSession)
SIMPLEMENT_DYNCREATE(DBSession)

SrvMgr*		DBSession::m_login		= new SrvMgr;
SrvMgr*		DBSession::m_lobby		= new SrvMgr;
SrvMgr*		DBSession::m_game		= new SrvMgr;

DBMgr*		DBSession::m_dbMgr		= &GetDBMgr;
PlayerMgr*	DBSession::m_playerMgr	= &GetPlayerMgr;

#ifdef _DEBUG
	SLogger*	DBSession::m_logger		= &GetLogger;
#endif
#ifdef CONNECT_LOG_SERVER
	LogSrvMgr*	DBSession::m_logSrv		= &GetLogSrvMgr;
#endif


DBSession::DBSession(void)
{
}

DBSession::~DBSession(void)
{
}

void DBSession::OnCreate()
{
	SSession::OnCreate();

	//���� ������.
	SendPacket( DB_TO_OTHER_CONNECT_OK );
}

void DBSession::OnDestroy()
{
	//���� �սõ�...

	SSession::OnDestroy();
}

void DBSession::PacketParsing( SPacket& packet )
{
	SSynchronize( this );

	switch( packet.GetID() )
	{
	case LOG_SERVER_CONNECT_OK:
		RecvLogSrvConnectOK();
		break;
	case LOG_SERVER_SHOTDOWN:
		//
		break;
	//==============================================================> Log����
	case SC_LOGIN_CONNECT_OK:
		RecvLoginConnectOK();
		break;
	case SC_LOBBY_CONNECT_OK:
		RecvLobbyConnectOK();
		break;
	case SC_GAME_CONNECT_OK:
		RecvGameConnectOK();
		break;
	//==============================================================> �ٸ� �������� ���� Ȯ��
	case LOGIN_TO_DB_CHECK_ID:
		RecvLoginCheckID( packet );
		break;
	case LOGIN_TO_DB_CREATE_ACCOUNT:
		RecvLoginCreateAccount( packet );
		break;
	case LOGIN_TO_DB_TRYLOGIN:
		RecvLoginTryLogin( packet );
		break;
	case LOBBY_TO_DB_CHARINSERT_READY_RESULT:
		RecvLobbyCharInsertReadyResult( packet );
		break;
	case GAME_TO_DB_UPDATE_USERDATA:
		//
		break;
	case OTHER_TO_DB_DISCONNECT_CHARACTER:
		RecvOtherSrvCharDisconnect( packet );
		break;
	default:
		//
		break;
	}
}

//==============================================================

void DBSession::RecvLogSrvConnectOK()
{
	m_logSrv->SetSession( this );

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
					_T("DBSession::RecvLogSrvConnectOK()\n�α� ������ ����Ǿ����ϴ�\n\n") );
#endif

}

//==============================================================

void DBSession::RecvLoginConnectOK()
{
	//�α��� ������ �����
	m_login->SetSession( this );

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
					_T("DBSession::RecvLoginConnectOK()\n�α��� ������ ����Ǿ����ϴ�\n\n") );
#endif

#ifdef CONNECT_LOG_SERVER
	m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
					_T("DBSession::RecvLoginConnectOK() �α��� ������ ����Ǿ����ϴ�") );
#endif
}

void DBSession::RecvLobbyConnectOK()
{
	//�κ񼭹��� �����
	m_lobby->SetSession( this );

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
					_T("DBSession::RecvLobbyConnectOK()\n�κ� ������ ����Ǿ����ϴ�\n\n") );
#endif

#ifdef CONNECT_LOG_SERVER
	m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
					_T("DBSession::RecvLobbyConnectOK() �κ� ������ ����Ǿ����ϴ�") );
#endif
}

void DBSession::RecvGameConnectOK()
{
	//���Ӽ����� �����
	m_game->SetSession( this );

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
					_T("DBSession::RecvGameConnectOK()\n���� ������ ����Ǿ����ϴ�\n\n") );
#endif

#ifdef CONNECT_LOG_SERVER
	m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
					_T("DBSession::RecvGameConnectOK() ���� ������ ����Ǿ����ϴ�") );
#endif
}

//--------------------------------------------------------------

void DBSession::RecvLoginCheckID( SPacket& packet )
{
	int indexId, size;
	TCHAR tmpId[10]={0,};

	packet >> indexId;
	//id����
	packet >> size;
	//�����ڵ�� 10���ڰ� size 20��
	if( size > 20 )
	{
		//id�ҷ�
		SendLoginCheckIDResult( indexId, FALSE );
		return;
	}
	packet.GetData( tmpId, size );

	//dbüũ
	BOOL result = m_dbMgr->CheckID( tmpId );

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
				_T("DBSession::RecvLoginCheckID()\nID %s�� �ߺ������ %s�Դϴ�.\n\n"), tmpId, result ? _T("��밡��") : _T("���Ұ���") );
#endif

#ifdef CONNECT_LOG_SERVER
	m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
					_T("DBSession::RecvLoginCheckID() ID %s�� �ߺ������ %s�Դϴ�."), tmpId, result ? _T("��밡��") : _T("���Ұ���") );
#endif

	//��� return
	SendLoginCheckIDResult( indexId, result );
}

void DBSession::RecvLoginCreateAccount( SPacket& packet )
{
	int indexId, size;
	TCHAR tmpId[10]={0,}, tmpPw[15]={0,}, tmpMail[20]={0,};

	packet >> indexId;
	//id����
	packet >> size;
	if( size > 20 )
	{
		//id�ҷ�
		SendLoginCreateAccountResult( indexId, FALSE );
		return;
	}
	packet.GetData( tmpId, size );
	//pw����
	packet >> size;
	if( size > 30 )
	{
		//pw�ҷ�
		SendLoginCreateAccountResult( indexId, FALSE );
		return;
	}
	packet.GetData( tmpId, size );
	//email����
	packet >> size;
	if( size > 40 )
	{
		//email�ҷ�
		SendLoginCreateAccountResult( indexId, FALSE );
		return;
	}
	packet.GetData( tmpId, size );

	//db�� ���� ����
	BOOL result = m_dbMgr->CreateAccount( tmpId, tmpPw, tmpMail );

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
					_T("DBSession::RecvLoginCreateAccount()\nID %s �������� ����� %s.\n\n"), tmpId, result ? _T("����") : _T("����") );
#endif

#ifdef CONNECT_LOG_SERVER
	m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
					_T("DBSession::RecvLoginCreateAccount() ID %s �������� ����� %s."), tmpId, result ? _T("����") : _T("����") );
#endif

	//��� return
	SendLoginCreateAccountResult( indexId, result );
}

void DBSession::RecvLoginTryLogin( SPacket& packet )
{
	int indexId, size;
	TCHAR tmpId[10]={0,}, tmpPw[15]={0,};

	packet >> indexId;
	//id����
	packet >> size;
	if( size > 20 )
	{
		//id�ҷ�
		SendLoginTryLoginResult( indexId, NONEXISTENT_ID );
		return;
	}
	packet.GetData( tmpId, size );
	//pw����
	packet >> size;
	if( size > 30 )
	{
		//pw�ҷ�
		SendLoginTryLoginResult( indexId, WRONG_PW );
		return;
	}
	packet.GetData( tmpPw, size );

	int sessionID = m_dbMgr->TryLogin( tmpId, tmpPw );

	if( sessionID == -1 )
	{
		//id����
		SendLoginTryLoginResult( indexId, NONEXISTENT_ID );
		return;
	}
	else if( sessionID == 0 )
	{
		//pwƲ��
		SendLoginTryLoginResult( indexId, WRONG_PW );
		return;
	}
	else if( sessionID == -10 )	
	{
		//��������
		SendLoginTryLoginResult( indexId, SERVER_ERROR );
		return;
	}


	//�α��ο� �����ߴٸ� �̹� �α��εǾ� �ִ� id������ �˻��Ѵ�.
	PlayerObj* tmpPlayer = m_playerMgr->FindPlayerBySessionId( sessionID );

	if( tmpPlayer == NULL )
	{
		//�������� �α���
		//ĳ���͸� ����
		tmpPlayer = m_playerMgr->GetPlayerSpace();
		m_dbMgr->SettingUserData( sessionID, tmpPlayer );
		m_playerMgr->AddPlayer( sessionID, tmpPlayer );

#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
						_T("DBSession::RecvLoginTryLogin()\nID %s �α��� ����/ �κ񼭹��� ĳ���� ������ ��û.\n\n"), tmpId );
#endif

#ifdef CONNECT_LOG_SERVER
		m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
						_T("DBSession::RecvLoginTryLogin() ID %s �α��� ����./ �κ񼭹��� ĳ���� ������ ��û"), tmpId );
#endif

		//�κ� �˸�
		SendLobbyChacterLogin( indexId, tmpPlayer );
	}
	else
	{
		//�̹� �α��� �Ǿ� �ִ� ID
		//�ٸ� ������ �ָ� ƨ���� ���Ѵ�
		//
		//
		//
#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
						_T("DBSession::RecvLoginTryLogin()\nID %s �̹� �α��� �� ID�Դϴ�.\n\n"), tmpId );
#endif

#ifdef CONNECT_LOG_SERVER
		m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
						_T("DBSession::RecvLoginTryLogin() ID %s �̹� �α��� �� ID�Դϴ�."), tmpId );
#endif

		//id�� �̹� ������̾��ٰ� ������
		SendLoginTryLoginResult( indexId, PRE_LOGIN_ID );

		//ĳ���� ����
		m_playerMgr->ReturnPlayerSpace( tmpPlayer );
	}
}

void DBSession::RecvLobbyCharInsertReadyResult( SPacket& packet )
{
	int index, sessionId;

	packet >> index;
	packet >> sessionId;

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
					_T("DBSession::RecvLobbyCharInsertReadyResult()\nSessionID %d�� �α��οϺ��ϰ� ����.\n\n"), sessionId );
#endif

#ifdef CONNECT_LOG_SERVER
	m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
					_T("DBSession::RecvLobbyCharInsertReadyResult() SessionID %d�� �α��οϺ��ϰ� ����."), sessionId );
#endif

	//���Ǿ��̵� ������� ����ִ� ���̴�
	SendLoginTryLoginResult( index, sessionId );
}

void DBSession::RecvOtherSrvCharDisconnect( SPacket& packet )
{
	int sessionId;

	packet >> sessionId;

	//id�� ĳ���͸� ã�� DB�� ������Ʈ �� �ش�
	PlayerObj* tmpPlayer = m_playerMgr->FindPlayerBySessionId( sessionId );
	if( tmpPlayer == NULL )
	{
#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
						_T("DBSession::RecvOtherSrvCharDisconnect()\nSessionID %d�� player������ ã�� �� �����ϴ�.\n\n"), sessionId );
#endif

		return;
	}

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
					_T("DBSession::RecvOtherSrvCharDisconnect()\nSessionID %d�� dataUPdate\n")
					_T("rankID: %d/ rankPoint: %d/ KillCount: %d/ DeathCount: %d\n\n")
					, sessionId
					, tmpPlayer->GetRankID(), tmpPlayer->GetRankPoint(), tmpPlayer->GetAccumulKillCount(), tmpPlayer->GetAccumulDeathCount() );
#endif

#ifdef CONNECT_LOG_SERVER
	m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
					_T("DBSession::RecvOtherSrvCharDisconnect()\nSessionID %d�� dataUPdate\t")
					_T("rankID: %d/ rankPoint: %d/ KillCount: %d/ DeathCount: %d")
					, sessionId
					, tmpPlayer->GetRankID(), tmpPlayer->GetRankPoint(), tmpPlayer->GetAccumulKillCount(), tmpPlayer->GetAccumulDeathCount() );
#endif
	
	if( !tmpPlayer->DBDataUpdate() )
	{
		//�α׸� �����.
#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
						_T("DBSession::RecvOtherSrvCharDisconnect()\nSessionID %d�� dataUPdate����.\n\n"), sessionId );
#endif

#ifdef CONNECT_LOG_SERVER
		m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
						_T("DBSession::RecvOtherSrvCharDisconnect() SessionID %d�� dataUPdate����."), sessionId );
#endif
	}

	m_playerMgr->ReturnPlayerSpace( tmpPlayer );
}

//==============================================================

BOOL DBSession::SendLoginCheckIDResult( int indexId, BOOL result )
{
	SPacket sendPacket( DB_TO_LOGIN_CHECK_ID_RESULT );

	sendPacket << indexId;
	sendPacket << result;

	SendPacket( sendPacket );
	
	return TRUE;
}

BOOL DBSession::SendLoginCreateAccountResult( int indexId, BOOL result )
{
	SPacket sendPacket( DB_TO_LOGIN_CREATE_ACCOUNT_RESULT );

	sendPacket << indexId;
	sendPacket << result;

	SendPacket( sendPacket );

	return TRUE;
}

BOOL DBSession::SendLobbyChacterLogin( int index, PlayerObj* player )
{
	SPacket sendPacket( DB_TO_LOBBY_CHARACTER_LOGIN );

	sendPacket << index;
	player->PackageMyAllInfo( sendPacket );

	m_lobby->SendPacketToSrv( sendPacket );

	return TRUE;
}

BOOL DBSession::SendLoginTryLoginResult( int indexId, int result )
{
	SPacket sendPacket( DB_TO_LOGIN_LOGINRESULT );

	sendPacket << indexId;
	sendPacket << result;

	m_login->SendPacketToSrv( sendPacket );

	return TRUE;
}

