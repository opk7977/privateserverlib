#include "DBSession.h"
#include "DBProtocol.h"

#include "SrvMgr.h"
#include "DBMgr.h"
#include "PlayerMgr.h"

SIMPLEMENT_DYNAMIC(DBSession)
SIMPLEMENT_DYNCREATE(DBSession)

SrvMgr*		DBSession::m_login		= new SrvMgr;
SrvMgr*		DBSession::m_lobby		= new SrvMgr;
SrvMgr*		DBSession::m_game		= new SrvMgr;

DBMgr*		DBSession::m_dbMgr		= &GetDBMgr;
PlayerMgr*	DBSession::m_playerMgr	= &GetPlayerMgr;

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
		//
		break;
	case LOG_SERVER_SHOTDOWN:
		//
		break;
	//==============================================================
	case SC_LOGIN_CONNECT_OK:
		RecvLoginConnectOK();
		break;
	case SC_LOBBY_CONNECT_OK:
		RecvLobbyConnectOK();
		break;
	case SC_GAME_CONNECT_OK:
		RecvGameConnectOK();
		break;
	//==============================================================
	case LOGIN_TO_DB_CHECK_ID:
		RecvLoginCheckID( packet );
		break;
	case LOGIN_TO_DB_CREATE_ACCOUNT:
		RecvLoginCreateAccount( packet );
		break;
	case LOGIN_TO_DB_TRYLOGIN:
		RecvLoginTryLogin( packet );
		break;
	case GAME_TO_DB_UPDATE_USERDATA:
		//
		break;
	case OTHER_TO_DB_DISCONNECT_CHARACTER:
		//
		break;
	default:
		//
		break;
	}
}

//==============================================================

void DBSession::RecvLoginConnectOK()
{
	//�α��� ������ �����
	m_login->SetSession( this );
}

void DBSession::RecvLobbyConnectOK()
{
	//�κ񼭹��� �����
	m_lobby->SetSession( this );
}

void DBSession::RecvGameConnectOK()
{
	//���Ӽ����� �����
	m_game->SetSession( this );
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
		//SendLoginTryLoginResult( indexId, NONEXISTENT_ID );
		return;
	}
	packet.GetData( tmpId, size );
	//pw����
	packet >> size;
	if( size > 30 )
	{
		//pw�ҷ�
		//SendLoginTryLoginResult( indexId, WRONG_PW );
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

		//�κ� �˸�
		//SendLobbyChacterLogin( tmpPlayer );

		//�α��μ����� ����� �˸�
		SendLoginTryLoginResult( indexId, sessionID );
	}
	else
	{
		//�̹� �α��� �Ǿ� �ִ� ID
		//�ٸ� ������ �ָ� ƨ���� ���Ѵ�
		//
		//
		//

		//id�� �̹� ������̾��ٰ� ������
		SendLoginTryLoginResult( indexId, PRE_LOGIN_ID );

		//ĳ���� ����
		m_playerMgr->ReturnPlayerSpace( tmpPlayer );
	}
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

BOOL DBSession::SendLobbyChacterLogin( PlayerObj* player )
{
	SPacket sendPacket( DB_TO_LOBBY_CHARACTER_LOGIN );

	player->PackageMyAllInfo( sendPacket );

	m_lobby->SendPacketToSrv( sendPacket );

	return TRUE;
}

BOOL DBSession::SendLoginTryLoginResult( int indexId, int result )
{
	SPacket sendPacket( DB_TO_LOGIN_LOGINRESULT );

	sendPacket << indexId;
	sendPacket << result;

	SendPacket( sendPacket );

	return TRUE;
}

